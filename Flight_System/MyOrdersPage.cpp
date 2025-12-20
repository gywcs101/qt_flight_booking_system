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
#include <ODBC.h>

MyOrdersPage::MyOrdersPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyOrdersPage)
{
    ui->setupUi(this);

    // 1. 去除边框 (AllFlightsPage 看起来比较干净，通常都有这句)
    ui->scrollArea->setFrameShape(QFrame::NoFrame);

    // 2. 强制设置滚动条样式表 (这是一个类似于携程/现代App的细长滚动条样式)
    // 如果你刚才在 UI 文件里没找到样式，把这段代码复制进去，两个界面都会变好看
    QString scrollStyle = R"(
        QScrollArea {
            border: none;
            background-color: transparent;
        }
        QScrollBar:vertical {
            border: none;
            background: #F5F5F5;
            width: 8px; /* 滚动条宽度 */
            margin: 0px 0 0px 0;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #CCCCCC; /* 滑块颜色 */
            min-height: 20px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: #999999; /* 鼠标悬停变深 */
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px; /* 隐藏上下的箭头 */
        }
    )";

    // 应用样式表
    ui->scrollArea->setStyleSheet(scrollStyle);

    // 3. 确保滚动区域背景透明 (防止白色遮挡)
    ui->scrollArea->setAttribute(Qt::WA_TranslucentBackground);
    ui->scrollArea->viewport()->setAttribute(Qt::WA_TranslucentBackground); // 关键
    ui->scrollAreaWidgetContents->setAttribute(Qt::WA_TranslucentBackground);

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

    // 清空旧数据
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) child->widget()->deleteLater();
        delete child;
    }

    int uid = UserSession::instance().getUserId();
    if (uid == -1) return;

    QString sql = QString("SELECT o.id as order_id, o.price as pay_price, f.* "
                          "FROM orders o "
                          "JOIN flights f ON o.flight_id = f.flight_id "
                          "WHERE o.user_id = %1 "
                          "ORDER BY o.create_time DESC").arg(uid);

    QSqlQuery query = ODBC::query(sql);

    if(query.isActive()) {
        int delayCounter = 0; // 【关键】延迟计数器

        while(query.next()) {
            FlightData data;
            data.flightId = query.value("flight_id").toString();
            data.airline = query.value("airline").toString();
            data.depCity = query.value("departure_city").toString();
            data.arrCity = query.value("arrival_city").toString();
            data.depTime = query.value("departure_time").toDateTime();
            data.arrTime = query.value("arrival_time").toDateTime();

            int orderId = query.value("order_id").toInt();
            double paidPrice = query.value("pay_price").toDouble();

            OrderCard *card = new OrderCard(data, orderId, paidPrice);
            layout->addWidget(card);

            connect(card, &OrderCard::refundClicked, this, &MyOrdersPage::handleRefund);
            connect(card, &OrderCard::changeClicked, this, &MyOrdersPage::handleChange);

            // =======================================================
            // 【核心调用】启动瀑布流动画
            // 第1张: 0ms, 第2张: 80ms, 第3张: 160ms...
            // =======================================================
            card->startEntryAnimation(delayCounter * 80);

            delayCounter++;
        }
        layout->addStretch();
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
