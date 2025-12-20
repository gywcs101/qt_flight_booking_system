#include "ChangeFlightDialog.h"
#include "ui_ChangeFlightDialog.h"
#include "FlightCard.h"
#include "FlightData.h"
#include "ODBC.h" // 【核心】使用你提供的 ODBC 类
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

ChangeFlightDialog::ChangeFlightDialog(QWidget *parent, QString oldFlightId, QString dep, QString arr, double oldPrice) :
    QDialog(parent), ui(new Ui::ChangeFlightDialog), m_oldFlightId(oldFlightId), m_oldPrice(oldPrice)
{
    ui->setupUi(this);
    this->setWindowTitle("选择改签航班");
    this->resize(600, 500);

    // 1. 初始化滚动区布局
    // 确保在UI文件里勾选了 scrollArea 的 widgetResizable
    ui->scrollArea->setWidgetResizable(true);

    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        layout->setSpacing(15);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setAlignment(Qt::AlignTop);
    }

    // 2. 加载数据
    loadAlternativeFlights(dep, arr);
}

ChangeFlightDialog::~ChangeFlightDialog() { delete ui; }

void ChangeFlightDialog::loadAlternativeFlights(QString dep, QString arr)
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());

    // 清空旧内容（如果有）
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    // [调试] 打印查询参数
    qDebug() << "[改签查询] 出发:" << dep << " 到达:" << arr << " 原航班:" << m_oldFlightId;

    // =================================================================
    // 【核心修改】构建 SQL 语句
    // 因为 ODBC::query 会直接执行，所以必须在这里把变量拼进去
    // 注意：'%1', '%2' 这种字符串变量外面要加单引号
    // =================================================================
    QString sql = QString(
                      "SELECT * FROM flights "
                      "WHERE departure_city = '%1' "   // 出发地
                      "AND arrival_city = '%2' "       // 目的地
                      "AND flight_id != '%3' "         // 排除当前航班
                      "AND departure_time > NOW() "    // 只能改签未来的航班
                      "ORDER BY departure_time ASC"
                      ).arg(dep).arg(arr).arg(m_oldFlightId);

    // 执行查询
    QSqlQuery query = ODBC::query(sql);

    // 检查是否出错
    if (!query.isActive()) {
        qDebug() << "改签查询失败:" << query.lastError().text();
        // 显示错误提示
        QLabel *errLabel = new QLabel("系统繁忙，无法加载航班信息", this);
        errLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(errLabel);
        return;
    }

    int count = 0;
    while (query.next()) {
        count++;
        FlightData data;
        // 填充数据
        data.flightId = query.value("flight_id").toString();
        data.airline = query.value("airline").toString();
        data.depCity = query.value("departure_city").toString();
        data.arrCity = query.value("arrival_city").toString();
        data.depTime = query.value("departure_time").toDateTime();
        data.arrTime = query.value("arrival_time").toDateTime();
        data.price = query.value("price").toDouble();
        data.status = query.value("status").toString();
        // 改签列表不需要显示是否收藏，默认false即可
        data.isFavorite = false;

        // 创建卡片
        FlightCard *card = new FlightCard(data, this);

        // 【关键】修改卡片行为
        // 断开原来的预订连接，连接到改签确认逻辑
        card->disconnect();

        // 也可以修改按钮文字（如果你的FlightCard有接口的话，没有也无所谓，点预订就是改签）
        // card->setButtonText("选择");

        connect(card, &FlightCard::bookClicked, [=](QString id){
            confirmChange(data);
        });

        layout->addWidget(card);
    }

    // 处理空结果
    if (count == 0) {
        QLabel *emptyLabel = new QLabel("抱歉，近期没有可改签的同航线航班。", this);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("color: #999; font-size: 16px; margin-top: 50px;");
        layout->addWidget(emptyLabel);
    }

    layout->addStretch();
}

void ChangeFlightDialog::confirmChange(const FlightData &newFlight)
{
    double diff = newFlight.price - m_oldPrice;
    QString msg;

    // 计算差价提示
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
        this->accept(); // 关闭对话框，返回 QDialog::Accepted
    }
}
