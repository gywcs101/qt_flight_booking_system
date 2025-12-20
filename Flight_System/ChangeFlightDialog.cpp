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
    this->resize(800, 600); // 窗口设大一点

    // ========================================================
    // 【核心修复 1】 强制设置 ScrollArea 属性
    // ========================================================
    ui->scrollArea->setWidgetResizable(true); // 让内容撑开
    ui->scrollArea->setStyleSheet("background-color: transparent; border: none;");
    ui->scrollAreaWidgetContents->setStyleSheet("background-color: transparent;");

    // ========================================================
    // 【核心修复 2】 强制检查并创建布局
    // 防止 UI 文件里忘记加垂直布局
    // ========================================================
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        layout->setSpacing(15);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setAlignment(Qt::AlignTop); // 必须顶端对齐
    }

    // 加载数据
    loadAlternativeFlights(dep, arr);
}

ChangeFlightDialog::~ChangeFlightDialog() { delete ui; }

void ChangeFlightDialog::loadAlternativeFlights(QString dep, QString arr)
{
    // 1. 获取滚动区的布局
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    // 2. 清空旧数据 (防止刷新时重复)
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    // 3. 获取当前时间 (格式化为 SQL 支持的字符串)
    QString nowStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 【后台显示】打印查询条件
    qDebug() << "==================================================";
    qDebug() << "【改签查询开始】";
    qDebug() << "出发地:" << dep;
    qDebug() << "目的地:" << arr;
    qDebug() << "排除原航班:" << m_oldFlightId;
    qDebug() << "时间要求 > " << nowStr;

    // 4. 构建 SQL 语句
    // 逻辑：同航线 + 排除原航班 + 时间在未来 + 按时间排序
    QString sql = QString(
                      "SELECT * FROM flights "
                      "WHERE departure_city = '%1' "
                      "AND arrival_city = '%2' "
                      "AND flight_id != '%3' "
                      "AND departure_time > '%4' "
                      "ORDER BY departure_time ASC"
                      ).arg(dep).arg(arr).arg(m_oldFlightId).arg(nowStr);

    // 5. 执行查询
    QSqlQuery query = ODBC::query(sql);

    // 检查查询是否执行成功
    if (!query.isActive()) {
        qDebug() << "【错误】SQL执行失败:" << query.lastError().text();
        qDebug() << "【错误】SQL语句:" << sql;

        QLabel *errLabel = new QLabel("系统繁忙，无法加载航班列表", this);
        errLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(errLabel);
        return;
    }

    // 6. 遍历结果并生成卡片
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
        data.isFavorite = false; // 改签列表不显示收藏状态

        // 【后台显示】打印找到的航班信息
        qDebug() << "--> 找到可改签航班:" << data.flightId
                 << "时间:" << data.depTime.toString("yyyy-MM-dd HH:mm")
                 << "价格:" << data.price;

        // 创建卡片
        FlightCard *card = new FlightCard(data, this);
        card->show(); // 强制显示，防止不可见

        // 劫持“预订”按钮，改为“确认改签”
        // 先断开它原本的连接，再连上我们要的逻辑
        card->disconnect();
        connect(card, &FlightCard::bookClicked, [=](QString id){
            confirmChange(data);
        });

        layout->addWidget(card);
    }

    qDebug() << "【改签查询结束】共找到" << count << "个航班";
    qDebug() << "==================================================";

    // 7. 处理没有结果的情况
    if (count == 0) {
        // 【后台显示】提示未找到
        qDebug() << "警告：数据库中没有符合条件的改签航班！";

        // 【前端UI显示】给用户提示
        QLabel *emptyLabel = new QLabel("抱歉，近期没有可改签的同航线航班。\n(请检查是否有未来日期的航班)", this);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("color: #999; font-size: 16px; margin-top: 50px; font-weight: bold;");
        layout->addWidget(emptyLabel);
    }

    // 8. 加底部弹簧
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
