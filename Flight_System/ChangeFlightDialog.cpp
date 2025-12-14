#include "ChangeFlightDialog.h"
#include "ui_ChangeFlightDialog.h"
#include "FlightCard.h"
#include "UserSession.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDebug>

ChangeFlightDialog::ChangeFlightDialog(QWidget *parent, QString oldFlightId, QString dep, QString arr, double oldPrice) :
    QDialog(parent), ui(new Ui::ChangeFlightDialog), m_oldFlightId(oldFlightId), m_oldPrice(oldPrice)
{
    ui->setupUi(this);
    this->setWindowTitle("选择改签航班");
    this->resize(600, 500);

    // 初始化布局
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        layout->setSpacing(10);
        layout->setContentsMargins(10, 10, 10, 10);
    }

    loadAlternativeFlights(dep, arr);
}

ChangeFlightDialog::~ChangeFlightDialog() { delete ui; }

void ChangeFlightDialog::loadAlternativeFlights(QString dep, QString arr)
{
    // 查找同航线，但不是当前航班的票
    // 这里为了演示，查的是所有未来日期的同航线
    QString sql = "SELECT * FROM flights WHERE departure_city = :dep AND arrival_city = :arr "
                  "AND flight_id != :oldId AND departure_time > NOW() ORDER BY departure_time ASC";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":dep", dep);
    query.bindValue(":arr", arr);
    query.bindValue(":oldId", m_oldFlightId);

    if (query.exec()) {
        QVBoxLayout *layout = (QVBoxLayout*)ui->scrollAreaWidgetContents->layout();
        while (query.next()) {
            FlightData data;
            // ... (填充数据逻辑同之前，省略) ...
            data.flightId = query.value("flight_id").toString();
            data.airline = query.value("airline").toString();
            data.depCity = query.value("departure_city").toString();
            data.arrCity = query.value("arrival_city").toString();
            data.depTime = query.value("departure_time").toDateTime();
            data.arrTime = query.value("arrival_time").toDateTime();
            data.price = query.value("price").toDouble();

            // 复用 FlightCard，但稍微修改行为
            FlightCard *card = new FlightCard(data);

            // 我们这里不需要收藏按钮，或者忽略它
            // 劫持预订按钮作为“选择改签”
            disconnect(card, nullptr, nullptr, nullptr); // 断开原有连接
            connect(card, &FlightCard::bookClicked, [=](QString id){
                confirmChange(data);
            });

            layout->addWidget(card);
        }
        layout->addStretch();
    }
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
        // 检查余额是否足够支付差价
        if (diff > 0) {
            // 这里简单查一下余额，实际应该在 MyOrdersPage 处理，但这里做个预检也可以
            // 为简单起见，我们直接确认，逻辑在主页面处理
        }
        m_selectedFlight = newFlight;
        m_confirmed = true;
        this->accept(); // 关闭对话框
    }
}
