#include "ChangeFlightDialog.h"
#include "ui_ChangeFlightDialog.h"
#include "FlightCard.h"
#include "FlightData.h"
#include "ODBC.h"
#include "UserSession.h" // 【修复】必须引入用户会话，否则没法存收藏
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

    // 1. 初始化布局
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setStyleSheet("background: transparent; border: none;");
    ui->scrollAreaWidgetContents->setStyleSheet("background: transparent;");

    if (ui->scrollAreaWidgetContents->layout()) {
        delete ui->scrollAreaWidgetContents->layout();
    }

    QVBoxLayout *layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setAlignment(Qt::AlignTop);

    // 2. 加载数据
    loadAlternativeFlights(dep, arr);
}

ChangeFlightDialog::~ChangeFlightDialog() { delete ui; }

void ChangeFlightDialog::loadAlternativeFlights(QString dep, QString arr)
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    // 【修复 1】获取当前用户ID
    int uid = UserSession::instance().getUserId();

    QString nowStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // =================================================================
    // 【修复 2】修改 SQL，增加 is_fav 字段查询
    // 这样如果航班已经是收藏状态，心形图标就会自动变红
    // =================================================================
    QString sql = QString(
                      "SELECT f.*, "
                      "(SELECT COUNT(*) FROM favorites WHERE user_id = %1 AND flight_id = f.flight_id) as is_fav "
                      "FROM flights f "
                      "WHERE departure_city = '%2' "
                      "AND arrival_city = '%3' "
                      "AND flight_id != '%4' "
                      "AND departure_time > '%5' "
                      "ORDER BY departure_time ASC"
                      ).arg(uid).arg(dep).arg(arr).arg(m_oldFlightId).arg(nowStr);

    QSqlQuery query = ODBC::query(sql);

    if (!query.isActive()) {
        qDebug() << "改签查询SQL失败:" << query.lastError().text();
        return;
    }

    int count = 0;
    int delayCounter = 0;

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

        // 【修复 3】正确读取收藏状态
        data.isFavorite = query.value("is_fav").toInt() > 0;

        FlightCard *card = new FlightCard(data, this);

        // 劫持预订按钮（改签逻辑）
        card->disconnect();
        connect(card, &FlightCard::bookClicked, [=](QString id){
            confirmChange(data);
        });

        // =================================================================
        // 【修复 4】添加收藏功能的信号连接
        // 之前这里漏掉了，所以点击没反应
        // =================================================================
        connect(card, &FlightCard::favClicked, [=](const QString& fid, bool isFav){
            if (uid == -1) return;

            QString favSql;
            if (isFav) {
                // 插入收藏
                favSql = QString("INSERT INTO favorites (user_id, flight_id) VALUES (%1, '%2')")
                             .arg(uid).arg(fid);
            } else {
                // 取消收藏
                favSql = QString("DELETE FROM favorites WHERE user_id = %1 AND flight_id = '%2'")
                             .arg(uid).arg(fid);
            }

            // 执行数据库操作
            ODBC::query(favSql);
            qDebug() << "改签界面更新收藏状态:" << fid << isFav;
        });
        // =================================================================

        layout->addWidget(card);
        card->startEntryAnimation(delayCounter * 50);
        delayCounter++;
    }

    if (count == 0) {
        QLabel *emptyLabel = new QLabel("没有找到可改签的航班", this);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("color: #999; font-size: 16px; margin-top: 50px;");
        layout->addWidget(emptyLabel);
    }

    layout->addStretch();
}

void ChangeFlightDialog::confirmChange(const FlightData &newFlight)
{
    // ... 保持原有逻辑不变 ...
    double diff = newFlight.price - m_oldPrice;
    QString msg;
    if (diff > 0) {
        msg = QString("改签至 %1\n新票价: ¥%2\n原票价: ¥%3\n\n需补差价: ¥%4\n确定支付并改签吗？")
                  .arg(newFlight.flightId).arg(newFlight.price).arg(m_oldPrice).arg(diff);
    } else if (diff < 0) {
        msg = QString("改签至 %1\n新票价: ¥%2\n原票价: ¥%3\n\n将退还差价: ¥%4\n确定改签吗？")
                  .arg(newFlight.flightId).arg(newFlight.price).arg(m_oldPrice).arg(qAbs(diff));
    } else {
        msg = QString("改签至 %1\n新票价: ¥%2\n原票价: ¥%3\n\n价格相同\n确定改签吗？")
                  .arg(newFlight.flightId).arg(newFlight.price).arg(m_oldPrice);
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "改签确认", msg, QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_selectedFlight = newFlight;
        m_confirmed = true;
        this->accept();
    }
}
