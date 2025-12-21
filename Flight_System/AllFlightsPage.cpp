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
#include <QMessageBox>
#include <QSqlDatabase>

AllFlightsPage::AllFlightsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllFlightsPage)
{
    ui->setupUi(this);

    // 1. 设置滚动区无边框，背景透明
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    ui->scrollArea->setAttribute(Qt::WA_TranslucentBackground);
    ui->scrollArea->viewport()->setAttribute(Qt::WA_TranslucentBackground);

    // 2. 现代风格滚动条样式表 (所有界面统一)
    QString commonStyle = R"(
        QScrollArea { border: none; background: transparent; }
        QScrollBar:vertical {
            border: none;
            background: #F5F5F5;      /* 浅灰轨道 */
            width: 8px;               /* 宽度8像素 */
            margin: 0px 0 0px 0;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #C0C0C0;      /* 银灰滑块 */
            min-height: 20px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: #909090;      /* 悬停变深 */
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;              /* 隐藏箭头 */
        }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
        }
    )";

    // 3. 应用样式
    ui->scrollArea->setStyleSheet(commonStyle);


    if (this->layout()) this->layout()->setContentsMargins(0,0,0,0);
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *vbox = new QVBoxLayout(ui->scrollAreaWidgetContents);
        vbox->setSpacing(15);
        vbox->setContentsMargins(20, 20, 20, 20);
    }
    ui->dateSelector->setDate(QDate::currentDate());
    connect(ui->btnSearch, &QPushButton::clicked, this, &AllFlightsPage::loadFlightsData);
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

    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    int uid = UserSession::instance().getUserId();

    //查找
    QString sql = QString("SELECT f.*, "
                          "(SELECT COUNT(*) FROM favorites WHERE user_id = %1 AND flight_id = f.flight_id) as is_fav "
                          "FROM flights f "
                          "WHERE f.flight_id NOT IN (SELECT flight_id FROM orders WHERE user_id = %1) "
                          ).arg(uid);

    // 拼接筛选条件
    if(!ui->lineEditDep->text().trimmed().isEmpty()) {
        sql += QString(" AND f.departure_city LIKE '%%1%'").arg(ui->lineEditDep->text().trimmed());
    }
    if(!ui->lineEditArr->text().trimmed().isEmpty()) {
        sql += QString(" AND f.arrival_city LIKE '%%1%'").arg(ui->lineEditArr->text().trimmed());
    }

    // 日期筛选
    QDate selDate = ui->dateSelector->getDate();
    sql += QString(" AND DATE(f.departure_time) = '%1'").arg(selDate.toString("yyyy-MM-dd"));

    // 排序
    sql += " ORDER BY f.departure_time ASC";

    // 执行查询
    QSqlQuery query = ODBC::query(sql);

    if (!query.isActive()) {
        qWarning() << "AllFlightsPage Query Failed:" << query.lastError().text();
        return;
    }

    int count = 0;
    int delayCounter = 0;

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

        //收藏
        connect(card, &FlightCard::favClicked, [=](const QString& fid, bool isFav){
            if (uid == -1) return;
            QString favSql;
            if (isFav) favSql = QString("INSERT INTO favorites (user_id, flight_id) VALUES (%1, '%2')").arg(uid).arg(fid);
            else favSql = QString("DELETE FROM favorites WHERE user_id = %1 AND flight_id = '%2'").arg(uid).arg(fid);
            ODBC::query(favSql);
        });

        //预定
        connect(card, &FlightCard::bookClicked, [=](QString fid) {
            if (uid == -1) {
                QMessageBox::warning(this, "提示", "请先登录后再预订机票！");
                return;
            }

            //检查余额
            QString balanceSql = QString("SELECT balance FROM users WHERE id = %1").arg(uid);
            QSqlQuery balanceQ = ODBC::query(balanceSql);
            if (balanceQ.next()) {
                double currentBalance = balanceQ.value("balance").toDouble();

                if (currentBalance < data.price) {
                    QMessageBox::warning(this, "余额不足",
                                         QString("您的余额 (¥%1) 不足支付 (¥%2)，请充值！").arg(currentBalance).arg(data.price));
                    return;
                }

                //开启事务
                QSqlDatabase::database().transaction();
                bool success = true;

                //扣款
                QString deductSql = QString("UPDATE users SET balance = balance - %1 WHERE id = %2")
                                        .arg(data.price).arg(uid);
                if (ODBC::query(deductSql).lastError().isValid()) success = false;

                //生成订单
                QString orderSql = QString("INSERT INTO orders (user_id, flight_id, price, status, create_time) "
                                           "VALUES (%1, '%2', %3, '已支付', NOW())")
                                       .arg(uid).arg(fid).arg(data.price);
                if (ODBC::query(orderSql).lastError().isValid()) success = false;

                //提交或回滚
                if (success) {
                    QSqlDatabase::database().commit();

                    QMessageBox::information(this, "预订成功",
                                             QString("预订成功！\n已自动扣除 ¥%1。\n您可以在“我的订单”中查看详情。").arg(data.price));

                    layout->removeWidget(card); //从布局移除
                    card->deleteLater();        //销毁对象
                } else {
                    QSqlDatabase::database().rollback();
                    QMessageBox::critical(this, "预订失败", "系统繁忙，交易已取消，请重试。");
                }
            }
        });
        card->startEntryAnimation(delayCounter * 80);
        delayCounter++;
    }

    if(count == 0){
        QLabel* tipLabel = new QLabel("未找到符合条件的航班\n(或者该日期的航班您已全部预订)", this);
        tipLabel->setAlignment(Qt::AlignCenter);
        tipLabel->setStyleSheet("color: #999; font-size: 16px;");
        layout->addWidget(tipLabel);
    }

    layout->addStretch();
}
