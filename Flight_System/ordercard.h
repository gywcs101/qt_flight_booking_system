#ifndef ORDERCARD_H
#define ORDERCARD_H

#include <QWidget>
#include "FlightData.h"

// 【新增】动画需要的头文件
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>

class QPushButton;

class OrderCard : public QWidget {
    Q_OBJECT
public:
    explicit OrderCard(const FlightData &data, int orderId, double paidPrice, QWidget *parent = nullptr);

    // 【核心】启动入场动画，参数是延迟时间(毫秒)
    void startEntryAnimation(int delay);

signals:
    void refundClicked(int orderId, double price, QString flightId);
    void changeClicked(int orderId, QString oldFlightId, QString dep, QString arr, double oldPrice);

private:
    FlightData m_data;
    int m_orderId;
    double m_paidPrice;

    // 保存特效指针
    QGraphicsOpacityEffect* m_opacityEffect;

    void setupUi();
};

#endif // ORDERCARD_H
