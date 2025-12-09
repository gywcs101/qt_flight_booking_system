#include "AllFlightsPage.h"
#include "ui_AllFlightsPage.h"
#include "FlightCard.h"
#include "UserSession.h" // 引入用户会话
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AllFlightsPage::AllFlightsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllFlightsPage)
{
    ui->setupUi(this);

    // 【重要】移除自身内边距，确保填充
    if (this->layout()) this->layout()->setContentsMargins(0,0,0,0);

    // 修复 ScrollArea 布局
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *vbox = new QVBoxLayout(ui->scrollAreaWidgetContents);
        vbox->setSpacing(15);
        vbox->setContentsMargins(20, 20, 20, 20);
    }

    // 设置初始日期 (由于 DateSelector 逻辑，会自动设为今天)
    // ui->dateSelector->setDate(QDate::currentDate());

    connect(ui->btnSearch, &QPushButton::clicked, this, &AllFlightsPage::loadFlightsData);

    // 初始加载
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

    // 清除旧卡片
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    // 获取当前用户ID
    int uid = UserSession::instance().getUserId();
    if (uid == -1) qDebug() << "警告：未登录，收藏功能将无法保存";

    // SQL: 查询航班 + 收藏状态 (子查询)
    QString sql = "SELECT f.*, "
                  "(SELECT COUNT(*) FROM favorites WHERE user_id = :uid AND flight_id = f.flight_id) as is_fav "
                  "FROM flights f WHERE 1=1";

    if(!ui->lineEditDep->text().isEmpty())
        sql += QString(" AND f.departure_city LIKE '%%1%'").arg(ui->lineEditDep->text());
    if(!ui->lineEditArr->text().isEmpty())
        sql += QString(" AND f.arrival_city LIKE '%%1%'").arg(ui->lineEditArr->text());

    QDate selDate = ui->dateSelector->getDate();
    sql += QString(" AND DATE(f.departure_time) = '%1'").arg(selDate.toString("yyyy-MM-dd"));

    sql += " ORDER BY f.departure_time ASC";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":uid", uid);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    while (query.next()) {
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

        FlightCard *card = new FlightCard(data);
        layout->addWidget(card);

        // 处理收藏点击
        connect(card, &FlightCard::favClicked, [=](QString fid, bool isFav){
            if (uid == -1) return; // 未登录不操作

            QSqlQuery favQ;
            if (isFav)
                favQ.prepare("INSERT INTO favorites (user_id, flight_id) VALUES (:uid, :fid)");
            else
                favQ.prepare("DELETE FROM favorites WHERE user_id = :uid AND flight_id = :fid");

            favQ.bindValue(":uid", uid);
            favQ.bindValue(":fid", fid);

            if(!favQ.exec()) qDebug() << "Fav DB Error:" << favQ.lastError().text();
            else qDebug() << "Fav updated for" << fid;
        });
    }
    layout->addStretch();
}
