#include "MyOrdersPage.h"
#include "ui_MyOrdersPage.h"
#include "OrderCard.h"          // 你的订单卡片类
#include "ChangeFlightDialog.h" // 你的改签弹窗类
#include "UserSession.h"        // 用户会话
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDebug>

MyOrdersPage::MyOrdersPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyOrdersPage)
{
    ui->setupUi(this);

    // 1. 美化背景 (淡灰色，与主界面融合)
    this->setStyleSheet("QWidget#MyOrdersPage { background-color: #F5F7FA; }");
    ui->scrollArea->setStyleSheet("background: transparent; border: none;");
    ui->scrollAreaWidgetContents->setStyleSheet("background: transparent;");

    // 2. 确保滚动区内部有布局
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *vbox = new QVBoxLayout(ui->scrollAreaWidgetContents);
        vbox->setSpacing(15);
        vbox->setContentsMargins(20, 20, 20, 20); // 卡片离边缘的距离
        vbox->setAlignment(Qt::AlignTop);         // 顶端对齐
    }
}

MyOrdersPage::~MyOrdersPage()
{
    delete ui;
}

void MyOrdersPage::loadOrders()
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    // 1. 清空旧列表
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    // 2. 获取用户ID
    int uid = UserSession::instance().getUserId();
    if (uid == -1) return; // 未登录

    // 3. 查库：联表查询 (订单表 + 航班表)
    // 假设订单表叫 orders, 航班表叫 flights
    QString sql = "SELECT o.id as order_id, o.price as pay_price, f.* "
                  "FROM orders o "
                  "JOIN flights f ON o.flight_id = f.flight_id "
                  "WHERE o.user_id = :uid "
                  "ORDER BY o.create_time DESC";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":uid", uid);

    if (query.exec()) {
        while (query.next()) {
            FlightData data;
            // 填充数据...
            data.flightId = query.value("flight_id").toString();
            data.airline  = query.value("airline").toString();
            data.depCity  = query.value("departure_city").toString();
            data.arrCity  = query.value("arrival_city").toString();
            data.depTime  = query.value("departure_time").toDateTime();
            data.arrTime  = query.value("arrival_time").toDateTime();

            // 获取订单特有数据
            int orderId = query.value("order_id").toInt();
            double paidPrice = query.value("pay_price").toDouble(); // 实付价格

            // 创建卡片 (传入 orderId 和 实付价格)
            OrderCard *card = new OrderCard(data, orderId, paidPrice);
            layout->addWidget(card);

            // 连接信号：退票
            connect(card, &OrderCard::refundClicked, this, &MyOrdersPage::handleRefund);
            // 连接信号：改签
            connect(card, &OrderCard::changeClicked, this, &MyOrdersPage::handleChange);
        }
        layout->addStretch(); // 底部弹簧
    } else {
        qDebug() << "Load orders error:" << query.lastError().text();
    }
}

// === 退票逻辑 ===
void MyOrdersPage::handleRefund(int orderId, double price)
{
    auto reply = QMessageBox::question(this, "退票确认",
                                       QString("确定要退票吗？\n将退还 ¥%1 到您的余额。").arg(price),
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    int uid = UserSession::instance().getUserId();

    // 开启事务 (保证删单和退钱同时成功)
    QSqlDatabase::database().transaction();

    bool ok = true;
    QSqlQuery q;

    // 1. 删订单
    q.prepare("DELETE FROM orders WHERE id = :oid");
    q.bindValue(":oid", orderId);
    if (!q.exec()) ok = false;

    // 2. 退钱 (update users set balance = balance + price)
    if (ok) {
        q.prepare("UPDATE users SET balance = balance + :amt WHERE id = :uid");
        q.bindValue(":amt", price);
        q.bindValue(":uid", uid);
        if (!q.exec()) ok = false;
    }

    if (ok) {
        QSqlDatabase::database().commit();
        QMessageBox::information(this, "成功", "退票成功！");
        loadOrders(); // 刷新列表
    } else {
        QSqlDatabase::database().rollback();
        QMessageBox::warning(this, "失败", "操作失败，请重试");
    }
}

// === 改签逻辑 ===
void MyOrdersPage::handleChange(int orderId, QString oldFlightId, QString dep, QString arr, double oldPrice)
{
    // 弹出改签对话框
    ChangeFlightDialog dialog(this, oldFlightId, dep, arr, oldPrice);

    if (dialog.exec() == QDialog::Accepted) {
        // 获取用户选的新航班
        FlightData newFlight = dialog.getSelectedFlight();
        double diff = newFlight.price - oldPrice; // 差价 (正数=补钱，负数=退钱)
        int uid = UserSession::instance().getUserId();

        QSqlDatabase::database().transaction();
        bool ok = true;
        QSqlQuery q;

        // 1. 处理钱 (补差价或退差价)
        if (diff > 0) {
            // 补钱：先查余额够不够
            q.prepare("SELECT balance FROM users WHERE id = :uid");
            q.bindValue(":uid", uid);
            q.exec(); q.next();
            if (q.value(0).toDouble() < diff) {
                QSqlDatabase::database().rollback();
                QMessageBox::warning(this, "余额不足", "余额不足以支付改签差价！");
                return;
            }
            // 扣钱
            q.prepare("UPDATE users SET balance = balance - :amt WHERE id = :uid");
            q.bindValue(":amt", diff);
            q.bindValue(":uid", uid);
            if(!q.exec()) ok = false;
        }
        else if (diff < 0) {
            // 退钱
            q.prepare("UPDATE users SET balance = balance + :amt WHERE id = :uid");
            q.bindValue(":amt", qAbs(diff)); // 取绝对值
            q.bindValue(":uid", uid);
            if(!q.exec()) ok = false;
        }

        // 2. 更新订单 (改 flight_id 和 price)
        if (ok) {
            q.prepare("UPDATE orders SET flight_id = :fid, price = :p WHERE id = :oid");
            q.bindValue(":fid", newFlight.flightId);
            q.bindValue(":p", newFlight.price);
            q.bindValue(":oid", orderId);
            if(!q.exec()) ok = false;
        }

        if (ok) {
            QSqlDatabase::database().commit();
            QMessageBox::information(this, "成功", "改签成功！");
            loadOrders();
        } else {
            QSqlDatabase::database().rollback();
            QMessageBox::warning(this, "失败", "改签失败，数据库错误");
        }
    }
}
