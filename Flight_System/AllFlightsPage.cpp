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

    // 1. 设置 ScrollArea 无边框，美观
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    // 给内部容器设置垂直布局 (这步建议在 Designer 里右键 scrollAreaWidgetContents -> Layout Vertical 完成)
    // 如果 Designer 没做，代码补救：
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *vbox = new QVBoxLayout(ui->scrollAreaWidgetContents);
        vbox->setSpacing(15);
        vbox->setContentsMargins(20, 10, 20, 10);
    }

    // 2. 初始日期设为今天
    ui->dateSelector->setDate(QDate::currentDate());

    // 3. 信号连接
    // 点击查询按钮 -> 刷新
    connect(ui->btnSearch, &QPushButton::clicked, this, &AllFlightsPage::loadFlightsData);

    // (可选) 日期一变自动刷新 -> 像携程一样
    connect(ui->dateSelector, &DateSelector::dateChanged, this, &AllFlightsPage::loadFlightsData);

    // 4. 初始加载
    loadFlightsData();
}

AllFlightsPage::~AllFlightsPage()
{
    delete ui;
}

void AllFlightsPage::loadFlightsData()
{
    // 获取布局
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    // 1. 清空旧卡片
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    // 2. 准备 SQL
    // 注意：DATE(departure_time) 是 MySQL 函数，用于提取 datetime 中的日期部分
    QString sql = "SELECT * FROM flights WHERE 1=1";

    // 筛选出发地
    if(!ui->lineEditDep->text().isEmpty()) {
        sql += QString(" AND departure_city LIKE '%%1%'").arg(ui->lineEditDep->text());
    }
    // 筛选目的地
    if(!ui->lineEditArr->text().isEmpty()) {
        sql += QString(" AND arrival_city LIKE '%%1%'").arg(ui->lineEditArr->text());
    }

    // 筛选日期 [关键修改]
    // 从自定义控件获取日期
    QDate selectedDate = ui->dateSelector->getDate();
    QString dateStr = selectedDate.toString("yyyy-MM-dd");
    sql += QString(" AND DATE(departure_time) = '%1'").arg(dateStr);

    // 排序
    sql += " ORDER BY departure_time ASC";

    // 3. 执行查询
    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "SQL Error:" << query.lastError().text();
        return;
    }

    // 4. 生成卡片
    while (query.next()) {
        FlightData data;
        data.flightId    = query.value("flight_id").toString();
        data.airline     = query.value("airline").toString();
        data.depCity     = query.value("departure_city").toString();
        data.arrCity     = query.value("arrival_city").toString();
        data.depTime     = query.value("departure_time").toDateTime();
        data.arrTime     = query.value("arrival_time").toDateTime();
        data.price       = query.value("price").toDouble();
        data.status      = query.value("status").toString();

        // 创建并添加卡片
        FlightCard *card = new FlightCard(data);
        layout->addWidget(card);

        connect(card, &FlightCard::bookClicked, [=](QString id){
            qDebug() << "Book:" << id;
        });
    }

    layout->addStretch();
}
