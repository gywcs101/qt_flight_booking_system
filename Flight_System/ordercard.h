#ifndef ORDERCARD_H
#define ORDERCARD_H

#include <QWidget>
#include "FlightData.h" // 必须包含这个，否则不认识 FlightData

class QPushButton;

class OrderCard : public QWidget {
    Q_OBJECT
public:
    // 构造函数声明
    explicit OrderCard(const FlightData &data, int orderId, double paidPrice, QWidget *parent = nullptr);

signals:
    void refundClicked(int orderId, double price, QString flightId);
    void changeClicked(int orderId, QString oldFlightId, QString dep, QString arr, double oldPrice);

private:
    // ！！！这些成员变量必须声明，否则cpp里全是红线！！！
    FlightData m_data;
    int m_orderId;
    double m_paidPrice;

    // ！！！setupUi 函数也必须声明！！！
    void setupUi();
};

#endif // ORDERCARD_H
