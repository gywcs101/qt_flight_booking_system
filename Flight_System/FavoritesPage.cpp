#include "FavoritesPage.h"
#include "ui_FavoritesPage.h"
#include "FlightCard.h"
#include "FlightData.h"
#include "UserSession.h"
#include "ODBC.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>    // [新增]
#include <QSqlDatabase>   // [新增]

FavoritesPage::FavoritesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FavoritesPage)
{
    ui->setupUi(this);

    // 样式设置
    this->setStyleSheet("QWidget#FavoritesPage { background-color: #F5F7FA; }");
    ui->scrollArea->setStyleSheet("background: transparent; border: none;");
    ui->scrollAreaWidgetContents->setStyleSheet("background: transparent;");

    // 布局初始化
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *vbox = new QVBoxLayout(ui->scrollAreaWidgetContents);
        vbox->setSpacing(15);
        vbox->setContentsMargins(20, 0, 20, 20);
        vbox->setAlignment(Qt::AlignTop);
    }
}

FavoritesPage::~FavoritesPage()
{
    delete ui;
}

void FavoritesPage::loadFavoriteFlights()
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    // 清空
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) child->widget()->deleteLater();
        delete child;
    }

    int uid = UserSession::instance().getUserId();
    if (uid == -1) {
        showEmptyState(layout, "请先登录查看收藏");
        layout->addStretch();
        return;
    }

    QString sql = QString(
                      "SELECT f.* FROM flights f "
                      "INNER JOIN favorites fav ON f.flight_id = fav.flight_id "
                      "WHERE fav.user_id = %1 "
                      "ORDER BY fav.create_time DESC"
                      ).arg(uid);

    QSqlQuery query = ODBC::query(sql);

    if (!query.isActive()) {
        qDebug() << "Fav Query Failed:" << query.lastError().text();
        showEmptyState(layout, "数据加载失败");
        return;
    }

    int count = 0;
    while (query.next()) {
        count++;
        FlightData data;
        data.flightId = query.value("flight_id").toString();
        data.airline  = query.value("airline").toString();
        data.depCity  = query.value("departure_city").toString();
        data.arrCity  = query.value("arrival_city").toString();
        data.depTime  = query.value("departure_time").toDateTime();
        data.arrTime  = query.value("arrival_time").toDateTime();
        data.price    = query.value("price").toDouble();
        data.status   = query.value("status").toString();
        data.isFavorite = true;

        FlightCard *card = new FlightCard(data, this);
        layout->addWidget(card);

        // --- 1. 取消收藏逻辑 ---
        connect(card, &FlightCard::favClicked, [=](const QString& fid, bool isFavNow){
            if (!isFavNow) {
                QString deleteSql = QString("DELETE FROM favorites WHERE user_id = %1 AND flight_id = '%2'").arg(uid).arg(fid);
                ODBC::query(deleteSql);
                card->hide();
                card->deleteLater();
                // 简单处理：不重新检查是否为空
            }
        });

        // --- 2. 预订逻辑 (与 AllFlightsPage 逻辑完全一致) ---
        connect(card, &FlightCard::bookClicked, [=](QString fid) {
            QString balanceSql = QString("SELECT balance FROM users WHERE id = %1").arg(uid);
            QSqlQuery balanceQ = ODBC::query(balanceSql);
            if (balanceQ.next()) {
                double currentBalance = balanceQ.value("balance").toDouble();

                if (currentBalance < data.price) {
                    QMessageBox::warning(this, "余额不足",
                                         QString("您的余额 (¥%1) 不足支付 (¥%2)，请充值！").arg(currentBalance).arg(data.price));
                    return;
                }

                QSqlDatabase::database().transaction();
                bool success = true;

                // 扣钱
                QString deductSql = QString("UPDATE users SET balance = balance - %1 WHERE id = %2")
                                        .arg(data.price).arg(uid);
                if (ODBC::query(deductSql).lastError().isValid()) success = false;

                // 生成订单
                QString orderSql = QString("INSERT INTO orders (user_id, flight_id, price, status, create_time) "
                                           "VALUES (%1, '%2', %3, '已支付', NOW())")
                                       .arg(uid).arg(fid).arg(data.price);
                if (ODBC::query(orderSql).lastError().isValid()) success = false;

                if (success) {
                    QSqlDatabase::database().commit();
                    QMessageBox::information(this, "预订成功",
                                             QString("预订成功！\n已扣除 ¥%1。\n请前往“我的订单”查看。").arg(data.price));
                } else {
                    QSqlDatabase::database().rollback();
                    QMessageBox::critical(this, "预订失败", "交易异常，请重试。");
                }
            }
        });
    }

    if (count == 0) {
        showEmptyState(layout, "您还没有收藏任何航班\n快去添加一些吧！");
    }
    layout->addStretch();
}

void FavoritesPage::showEmptyState(QVBoxLayout *layout, QString text)
{
    QWidget *w = new QWidget;
    QVBoxLayout *v = new QVBoxLayout(w);
    QLabel *icon = new QLabel("📂");
    icon->setStyleSheet("font-size: 60px; color: #CCC;"); icon->setAlignment(Qt::AlignCenter);
    QLabel *msg = new QLabel(text);
    msg->setStyleSheet("font-size: 16px; color: #999;"); msg->setAlignment(Qt::AlignCenter);
    v->addStretch(); v->addWidget(icon); v->addWidget(msg); v->addStretch();
    layout->addWidget(w);
}
