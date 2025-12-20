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
    this->resize(850, 600); // 窗口再大一点

    // =================================================================
    // 【BUG 修复区】 强制设置 UI 属性，无视 .ui 文件的错误设置
    // =================================================================

    // 1. 强制让滚动区的内容自动撑开 (如果不写这句，内容区高度可能是0，导致一片白)
    ui->scrollArea->setWidgetResizable(true);

    // 2. 暴力重置布局：如果有旧布局先删掉，重新 new 一个
    // 这能解决“明明有数据但是界面不显示”或者“显示成一坨线”的问题
    if (ui->scrollAreaWidgetContents->layout()) {
        delete ui->scrollAreaWidgetContents->layout();
    }

    // 3. 创建全新的垂直布局
    QVBoxLayout *layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setAlignment(Qt::AlignTop); // 顶端对齐

    // =================================================================

    // 加载数据
    loadAlternativeFlights(dep, arr);
}

ChangeFlightDialog::~ChangeFlightDialog() { delete ui; }

void ChangeFlightDialog::loadAlternativeFlights(QString dep, QString arr)
{
    // 获取刚才我们在构造函数里强制 new 的布局
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());

    // 防御性检查 (虽然上面new了，但为了安全)
    if (!layout) return;

    // 清空旧数据
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    QString nowStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 打印调试信息
    qDebug() << "========================================";
    qDebug() << "【正在查找改签航班】";
    qDebug() << "出发:" << dep << " 到达:" << arr;
    qDebug() << "时间 > " << nowStr;

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
        return;
    }

    int count = 0;
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

        // 打印查到的数据，确保存活
        qDebug() << "--> 生成卡片:" << data.flightId << " 价格:" << data.price;

        FlightCard *card = new FlightCard(data, this);

        // 【关键】强制卡片可见，并设置固定高度 (防止被压缩成0)
        card->show();
        card->setMinimumHeight(120);

        // 劫持点击事件
        card->disconnect();
        connect(card, &FlightCard::bookClicked, [=](QString id){
            confirmChange(data);
        });

        layout->addWidget(card);
    }

    qDebug() << "共生成" << count << "张卡片";
    qDebug() << "========================================";

    if (count == 0) {
        QLabel *emptyLabel = new QLabel("暂无符合条件的改签航班", this);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("color: #999; font-size: 18px; margin-top: 50px;");
        layout->addWidget(emptyLabel);
    }

    layout->addStretch();
}

void ChangeFlightDialog::confirmChange(const FlightData &newFlight)
{
    double diff = newFlight.price - m_oldPrice;
    QString msg;
    if (diff > 0) {
        msg = QString("改签至 %1\n新票价: ¥%2\n原票价: ¥%3\n\n需补差价: ¥%4\n确定支付并改签吗？")
                  .arg(newFlight.flightId).arg(newFlight.price).arg(m_oldPrice).arg(diff);
    } else {
        msg = QString("改签至 %1\n新票价: ¥%2\n原票价: ¥%3\n\n将退还差价: ¥%4\n确定改签吗？")
                  .arg(newFlight.flightId).arg(newFlight.price).arg(m_oldPrice).arg(qAbs(diff));
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "改签确认", msg, QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_selectedFlight = newFlight;
        m_confirmed = true;
        this->accept();
    }
}
