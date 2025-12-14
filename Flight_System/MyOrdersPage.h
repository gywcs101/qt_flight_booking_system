#ifndef MYORDERSPAGE_H
#define MYORDERSPAGE_H

#include <QWidget>

namespace Ui {
class MyOrdersPage;
}

class MyOrdersPage : public QWidget
{
    Q_OBJECT

public:
    explicit MyOrdersPage(QWidget *parent = nullptr);
    ~MyOrdersPage();

    // 供 MainWindow 调用的刷新函数
    void loadOrders();

private:
    Ui::MyOrdersPage *ui;

    // 处理退票逻辑
    void handleRefund(int orderId, double price);
    // 处理改签逻辑
    void handleChange(int orderId, QString oldFlightId, QString dep, QString arr, double oldPrice);
};

#endif // MYORDERSPAGE_H
