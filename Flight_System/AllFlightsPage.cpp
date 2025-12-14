#include "AllFlightsPage.h"
#include "ui_AllFlightsPage.h"
#include "FlightCard.h"
#include "FlightData.h"      // 确保包含 FlightData
#include "UserSession.h"
#include "DateSelector.h"
#include "ODBC.h"            // [核心修改] 引入全局 ODBC 类
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QLayoutItem>

AllFlightsPage::AllFlightsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllFlightsPage)
{
    ui->setupUi(this);

    // 优化布局
    if (this->layout()) this->layout()->setContentsMargins(0,0,0,0);
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *vbox = new QVBoxLayout(ui->scrollAreaWidgetContents);
        vbox->setSpacing(15);
        vbox->setContentsMargins(20, 20, 20, 20);
    }

    // 设置默认日期为今天
    ui->dateSelector->setDate(QDate::currentDate());

    // 连接搜索按钮的点击信号到加载数据的槽函数
    connect(ui->btnSearch, &QPushButton::clicked, this, &AllFlightsPage::loadFlightsData);

    // 界面初次显示时，自动加载一次数据
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

    // 清空旧卡片
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    // 获取当前登录用户ID
    int uid = UserSession::instance().getUserId();

    // 基础SQL语句：联表查询收藏状态
    QString sql = QString("SELECT f.*, "
                          "(SELECT COUNT(*) FROM favorites WHERE user_id = %1 AND flight_id = f.flight_id) as is_fav "
                          "FROM flights f WHERE 1=1").arg(uid);

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

    sql += " ORDER BY f.departure_time ASC";

    // [核心修改] 使用 ODBC 类执行查询
    QSqlQuery query = ODBC::query(sql);

    if (!query.isActive()) {
        qWarning() << "AllFlightsPage: Query Failed:" << query.lastError().text();
        return;
    }

    int count = 0; // 记录找到的航班数量
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

        // 连接收藏按钮的点击信号
        connect(card, &FlightCard::favClicked, [=](const QString& fid, bool isFav){
            if (uid == -1) {
                // 未登录处理
                qDebug() << "User not logged in, cannot change favorite status.";
                return;
            }

            QString favSql;
            if (isFav) {
                favSql = QString("INSERT INTO favorites (user_id, flight_id) VALUES (%1, '%2')").arg(uid).arg(fid);
            } else {
                favSql = QString("DELETE FROM favorites WHERE user_id = %1 AND flight_id = '%2'").arg(uid).arg(fid);
            }
            // 使用 ODBC 类执行更新
            ODBC::query(favSql);
        });
    }

    // 如果没有找到航班，可以给个提示
    if(count == 0){
        QLabel* tipLabel = new QLabel("未找到符合条件的航班", this);
        tipLabel->setAlignment(Qt::AlignCenter);
        tipLabel->setStyleSheet("color: #999; font-size: 16px;");
        layout->addWidget(tipLabel);
    }

    layout->addStretch(); // 添加弹簧，把卡片顶上去
}
