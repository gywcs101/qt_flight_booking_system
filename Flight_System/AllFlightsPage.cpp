#include "AllFlightsPage.h"
#include "ui_AllFlightsPage.h"
#include "FlightCard.h"
#include "FlightData.h"
#include "UserSession.h"
#include "DateSelector.h"
#include "ODBC.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QMessageBox>    // [新增] 弹窗
#include <QSqlDatabase>   // [新增] 事务

AllFlightsPage::AllFlightsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllFlightsPage)
{
    ui->setupUi(this);

    // 1. 布局优化
    if (this->layout()) this->layout()->setContentsMargins(0,0,0,0);
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *vbox = new QVBoxLayout(ui->scrollAreaWidgetContents);
        vbox->setSpacing(15);
        vbox->setContentsMargins(20, 20, 20, 20);
    }

    // 2. 初始化日期为今天
    ui->dateSelector->setDate(QDate::currentDate());

    // 3. 连接查询按钮
    connect(ui->btnSearch, &QPushButton::clicked, this, &AllFlightsPage::loadFlightsData);

    // 4. 自动加载一次
    loadFlightsData();
}

AllFlightsPage::~AllFlightsPage()
{
    delete ui;
}

void AllFlightsPage::loadFlightsData()
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    // 清空界面
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    int uid = UserSession::instance().getUserId();

    // SQL: 联表查询收藏状态
    QString sql = QString("SELECT f.*, "
                          "(SELECT COUNT(*) FROM favorites WHERE user_id = %1 AND flight_id = f.flight_id) as is_fav "
                          "FROM flights f WHERE 1=1").arg(uid);

    // 筛选条件
    if(!ui->lineEditDep->text().trimmed().isEmpty()) {
        sql += QString(" AND f.departure_city LIKE '%%1%'").arg(ui->lineEditDep->text().trimmed());
    }
    if(!ui->lineEditArr->text().trimmed().isEmpty()) {
        sql += QString(" AND f.arrival_city LIKE '%%1%'").arg(ui->lineEditArr->text().trimmed());
    }

    // 日期筛选
    QDate selDate = ui->dateSelector->getDate();
    sql += QString(" AND DATE(f.departure_time) = '%1'").arg(selDate.toString("yyyy-MM-dd"));
    sql += " ORDER BY f.departure_time ASC";

    // 执行查询
    QSqlQuery query = ODBC::query(sql);

    if (!query.isActive()) {
        qWarning() << "Query Failed:" << query.lastError().text();
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
        data.isFavorite = query.value("is_fav").toInt() > 0;

        FlightCard *card = new FlightCard(data, this);
        layout->addWidget(card);

        // --- 1. 收藏逻辑 ---
        connect(card, &FlightCard::favClicked, [=](const QString& fid, bool isFav){
            if (uid == -1) return;
            QString favSql;
            if (isFav) favSql = QString("INSERT INTO favorites (user_id, flight_id) VALUES (%1, '%2')").arg(uid).arg(fid);
            else favSql = QString("DELETE FROM favorites WHERE user_id = %1 AND flight_id = '%2'").arg(uid).arg(fid);
            ODBC::query(favSql);
        });

        // --- 2. 预订逻辑 (自动扣费 + 写入订单) ---
        connect(card, &FlightCard::bookClicked, [=](QString fid) {
            if (uid == -1) {
                QMessageBox::warning(this, "提示", "请先登录后再预订机票！");
                return;
            }

            // A. 检查余额
            QString balanceSql = QString("SELECT balance FROM users WHERE id = %1").arg(uid);
            QSqlQuery balanceQ = ODBC::query(balanceSql);
            if (balanceQ.next()) {
                double currentBalance = balanceQ.value("balance").toDouble();

                // 余额不足
                if (currentBalance < data.price) {
                    QMessageBox::warning(this, "余额不足",
                                         QString("您的余额 (¥%1) 不足支付 (¥%2)，请充值！").arg(currentBalance).arg(data.price));
                    return;
                }

                // B. 开启事务 (确保扣钱和生成订单同时成功)
                QSqlDatabase::database().transaction();
                bool success = true;

                // Step 1: 扣款
                QString deductSql = QString("UPDATE users SET balance = balance - %1 WHERE id = %2")
                                        .arg(data.price).arg(uid);
                QSqlQuery qDeduct = ODBC::query(deductSql);
                if (qDeduct.lastError().isValid()) success = false;

                // Step 2: 生成订单
                QString orderSql = QString("INSERT INTO orders (user_id, flight_id, price, status, create_time) "
                                           "VALUES (%1, '%2', %3, '已支付', NOW())")
                                       .arg(uid).arg(fid).arg(data.price);
                QSqlQuery qOrder = ODBC::query(orderSql);
                if (qOrder.lastError().isValid()) success = false;

                // C. 提交事务
                if (success) {
                    QSqlDatabase::database().commit();
                    QMessageBox::information(this, "预订成功",
                                             QString("预订成功！\n已扣除 ¥%1。\n请前往“我的订单”查看。").arg(data.price));
                } else {
                    QSqlDatabase::database().rollback(); // 失败回滚，钱不扣
                    QMessageBox::critical(this, "预订失败", "交易异常，请重试。");
                    qDebug() << "Booking Error:" << qDeduct.lastError() << qOrder.lastError();
                }
            }
        });
    }

    if(count == 0){
        QLabel* tipLabel = new QLabel("未找到符合条件的航班", this);
        tipLabel->setAlignment(Qt::AlignCenter);
        tipLabel->setStyleSheet("color: #999; font-size: 16px;");
        layout->addWidget(tipLabel);
    }

    layout->addStretch();
}
