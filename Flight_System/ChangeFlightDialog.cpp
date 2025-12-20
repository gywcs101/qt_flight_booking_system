#include "ChangeFlightDialog.h"
#include "ui_ChangeFlightDialog.h"
#include "FlightCard.h"
#include "FlightData.h"
#include "ODBC.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QDateTime>

ChangeFlightDialog::ChangeFlightDialog(QWidget *parent, QString oldFlightId, QString dep, QString arr, double oldPrice) :
    QDialog(parent), ui(new Ui::ChangeFlightDialog), m_oldFlightId(oldFlightId), m_oldPrice(oldPrice)
{
    ui->setupUi(this);
    this->setWindowTitle("选择改签航班");
    this->resize(850, 600);

    // =========================================================
    // 1. 强制初始化 ScrollArea 属性，防止界面显示异常
    // =========================================================
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setStyleSheet("background: transparent; border: none;");
    ui->scrollAreaWidgetContents->setStyleSheet("background: transparent;");

    // 2. 暴力重置布局 (确保布局存在且参数正确)
    if (ui->scrollAreaWidgetContents->layout()) {
        delete ui->scrollAreaWidgetContents->layout();
    }

    QVBoxLayout *layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setAlignment(Qt::AlignTop);

    // 3. 加载数据
    loadAlternativeFlights(dep, arr);
}

ChangeFlightDialog::~ChangeFlightDialog() { delete ui; }

void ChangeFlightDialog::loadAlternativeFlights(QString dep, QString arr)
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    // 清空旧数据
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    QString nowStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    qDebug() << "【改签查询】出发:" << dep << " 到达:" << arr << " 排除:" << m_oldFlightId;

    // SQL: 查找同航线、非原航班、时间在未来的航班
    QString sql = QString(
                      "SELECT * FROM flights "
                      "WHERE departure_city = '%1' "
                      "AND arrival_city = '%2' "
                      "AND flight_id != '%3' "
                      "AND departure_time > '%4' "
                      "ORDER BY departure_time ASC"
                      ).arg(dep).arg(arr).arg(m_oldFlightId).arg(nowStr);

    QSqlQuery query = ODBC::query(sql);

    if (!query.isActive()) {
        qDebug() << "SQL错误:" << query.lastError().text();
        QLabel *err = new QLabel("系统繁忙，无法加载数据", this);
        err->setAlignment(Qt::AlignCenter);
        layout->addWidget(err);
        return;
    }

    int count = 0;
    int delayCounter = 0; // 【关键】动画延迟计数器

    while (query.next()) {
        count++;
        FlightData data;
        data.flightId = query.value("flight_id").toString();
        data.airline = query.value("airline").toString();
        data.depCity = query.value("departure_city").toString();
        data.arrCity = query.value("arrival_city").toString();
        data.depTime = query.value("departure_time").toDateTime();
        data.arrTime = query.value("arrival_time").toDateTime();
        data.price = query.value("price").toDouble();
        data.status = query.value("status").toString();
        data.isFavorite = false;

        // 创建卡片 (此时它因为构造函数里的设置，是全透明的)
        FlightCard *card = new FlightCard(data, this);

        // 劫持点击事件：改签不是预订
        card->disconnect();
        connect(card, &FlightCard::bookClicked, [=](QString id){
            confirmChange(data);
        });

        layout->addWidget(card);

        // =============================================================
        // 【核心修复】 必须调用动画函数，卡片才会从透明变成显示！
        // =============================================================
        card->startEntryAnimation(delayCounter * 50);
        delayCounter++;
        // =============================================================
    }

    if (count == 0) {
        QLabel *emptyLabel = new QLabel("没有找到可改签的航班\n(请确认是否有未来日期的同航线航班)", this);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("color: #999; font-size: 16px; margin-top: 50px; font-weight: bold;");
        layout->addWidget(emptyLabel);
    }

    layout->addStretch();
}

void ChangeFlightDialog::confirmChange(const FlightData &newFlight)
{
    double diff = newFlight.price - m_oldPrice;
    QString msg;
    if (diff > 0) {
        msg = QString("改签至航班: %1\n\n新票价: ¥%2\n原票价: ¥%3\n\n【需补差价: ¥%4】\n\n确定支付并改签吗？")
                  .arg(newFlight.flightId).arg(newFlight.price).arg(m_oldPrice).arg(diff);
    } else if (diff < 0) {
        msg = QString("改签至航班: %1\n\n新票价: ¥%2\n原票价: ¥%3\n\n【将退还差价: ¥%4】\n\n确定改签吗？")
                  .arg(newFlight.flightId).arg(newFlight.price).arg(m_oldPrice).arg(qAbs(diff));
    } else {
        msg = QString("改签至航班: %1\n\n价格相同，无需补差价。\n\n确定改签吗？")
                  .arg(newFlight.flightId);
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "改签确认", msg, QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_selectedFlight = newFlight;
        m_confirmed = true;
        this->accept();
    }
}
