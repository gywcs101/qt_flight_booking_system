#include "AllFlightsPage.h"
#include "ui_AllFlightsPage.h"
#include "FlightCard.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AllFlightsPage::AllFlightsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllFlightsPage)
{
    ui->setupUi(this);

    // 设置日期选择器默认值
    ui->dateSelector->setDate(QDate::currentDate());

    // 修复布局（如果需要）
    if(ui->scrollAreaWidgetContents->layout()) {
        ui->scrollAreaWidgetContents->layout()->setSpacing(15);
        ui->scrollAreaWidgetContents->layout()->setContentsMargins(20, 20, 20, 20);
    } else {
        // 如果Designer里没设布局，这里补救
        QVBoxLayout *vbox = new QVBoxLayout(ui->scrollAreaWidgetContents);
        vbox->setSpacing(15);
        vbox->setContentsMargins(20, 20, 20, 20);
    }

    // 连接查询按钮
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

    // 1. 清除旧数据
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    // ==========================================
    // 2. 准备 SQL 查询 (核心修改)
    // ==========================================

    // 【重要】这里假设当前登录用户ID为 1
    // 在实际项目中，你应该从全局变量或 User单例 中获取这个ID
    int currentUserId = 1;

    // 使用子查询：同时查出航班信息 + 该用户是否收藏了它
    // if_fav 字段：如果有记录则 >0，否则为 0
    QString sql = "SELECT f.*, "
                  "(SELECT COUNT(*) FROM favorites WHERE user_id = :uid AND flight_id = f.flight_id) as is_fav "
                  "FROM flights f WHERE 1=1";

    // 2.1 拼接筛选条件
    if(!ui->lineEditDep->text().isEmpty())
        sql += QString(" AND f.departure_city LIKE '%%1%'").arg(ui->lineEditDep->text());

    if(!ui->lineEditArr->text().isEmpty())
        sql += QString(" AND f.arrival_city LIKE '%%1%'").arg(ui->lineEditArr->text());

    // 日期筛选
    QDate selectedDate = ui->dateSelector->getDate();
    QString dateStr = selectedDate.toString("yyyy-MM-dd");
    sql += QString(" AND DATE(f.departure_time) = '%1'").arg(dateStr);

    sql += " ORDER BY f.departure_time ASC";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":uid", currentUserId); // 绑定用户ID

    if (!query.exec()) {
        qDebug() << "Load flights failed:" << query.lastError().text();
        return;
    }

    // 3. 生成卡片
    while (query.next()) {
        FlightData data;
        // 读取航班基础信息
        data.flightId    = query.value("flight_id").toString();
        data.airline     = query.value("airline").toString();
        data.depCity     = query.value("departure_city").toString();
        data.arrCity     = query.value("arrival_city").toString();
        data.depTime     = query.value("departure_time").toDateTime();
        data.arrTime     = query.value("arrival_time").toDateTime();
        data.price       = query.value("price").toDouble();
        data.status      = query.value("status").toString();

        // 【新增】读取收藏状态 (1=已收藏, 0=未收藏)
        data.isFavorite  = query.value("is_fav").toInt() > 0;

        // 创建卡片
        FlightCard *card = new FlightCard(data);
        layout->addWidget(card);

        // ==========================================
        // 4. 处理收藏逻辑 (核心修改)
        // ==========================================
        connect(card, &FlightCard::favClicked, [=](QString flightId, bool isFavNow){
            QSqlQuery favQuery;
            if (isFavNow) {
                // 执行收藏
                favQuery.prepare("INSERT INTO favorites (user_id, flight_id) VALUES (:uid, :fid)");
            } else {
                // 取消收藏
                favQuery.prepare("DELETE FROM favorites WHERE user_id = :uid AND flight_id = :fid");
            }
            favQuery.bindValue(":uid", currentUserId);
            favQuery.bindValue(":fid", flightId);

            if (!favQuery.exec()) {
                qDebug() << "收藏操作数据库失败:" << favQuery.lastError().text();
                // 在这里可以加代码让卡片变回原来的颜色，提示失败
            } else {
                qDebug() << (isFavNow ? "收藏成功" : "取消收藏成功") << flightId;
            }
        });

        // 处理预订点击
        connect(card, &FlightCard::bookClicked, [=](QString id){
            qDebug() << "点击预订:" << id;
        });
    }

    layout->addStretch();
}
