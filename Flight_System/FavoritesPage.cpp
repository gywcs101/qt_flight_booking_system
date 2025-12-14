#include "FavoritesPage.h"
#include "ui_FavoritesPage.h"
#include "FlightCard.h"
#include "FlightData.h"
#include "UserSession.h"
#include "ODBC.h"            // [核心] 引入全局 ODBC 类
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>

FavoritesPage::FavoritesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FavoritesPage)
{
    ui->setupUi(this);
    // ... 您的样式表代码 ...
    this->setStyleSheet("..."); // 省略您的QSS

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

    // 清空界面
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) child->widget()->deleteLater();
        delete child;
    }

    int uid = UserSession::instance().getUserId();
    qDebug() << "[FavoritesPage] Loading favorites for user ID:" << uid;

    if (uid == -1) {
        showEmptyState(layout, "请先登录查看收藏");
        layout->addStretch();
        return;
    }

    // [核心修改] 使用全局的 ODBC::query() 函数来执行查询
    QString sql = QString(
                      "SELECT f.* FROM flights f "
                      "INNER JOIN favorites fav ON f.flight_id = fav.flight_id "
                      "WHERE fav.user_id = %1 "
                      "ORDER BY fav.create_time DESC"
                      ).arg(uid);

    QSqlQuery query = ODBC::query(sql);

    // 检查查询是否成功
    if (!query.isActive()) {
        qWarning() << "[FavoritesPage] Query failed:" << query.lastError().text();
        showEmptyState(layout, "数据加载失败，请检查数据库");
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

        connect(card, &FlightCard::favClicked, [=](const QString& fid, bool isFavNow){
            if (!isFavNow) {
                // [核心修改] 使用 ODBC 类执行删除
                QString deleteSql = QString("DELETE FROM favorites WHERE user_id = %1 AND flight_id = '%2'").arg(uid).arg(fid);
                ODBC::query(deleteSql);
                qDebug() << "[FavoritesPage] Favorite removed:" << fid;

                // 立即从界面移除卡片
                card->hide();
                card->deleteLater();

                // 可以在这里重新检查是否为空并显示空状态，为了简化暂时省略
            }
        });

        // connect bookClicked...
    }

    if (count == 0) {
        showEmptyState(layout, "您还没有收藏任何航班\n快去添加一些吧！");
    }

    layout->addStretch();
}

void FavoritesPage::showEmptyState(QVBoxLayout *layout, QString text)
{
    // ... 您的代码保持不变 ...
    QWidget *emptyContainer = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout(emptyContainer);
    QLabel *icon = new QLabel("📂");
    icon->setStyleSheet("font-size: 60px;");
    icon->setAlignment(Qt::AlignCenter);
    QLabel *msg = new QLabel(text);
    msg->setStyleSheet("font-size: 16px; color: #909399;");
    msg->setAlignment(Qt::AlignCenter);
    vbox->addStretch();
    vbox->addWidget(icon);
    vbox->addWidget(msg);
    vbox->addStretch();
    layout->addWidget(emptyContainer);
}
