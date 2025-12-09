#ifndef FLIGHTCARD_H
#define FLIGHTCARD_H

#include <QWidget>
#include "FlightData.h"

class FlightCard : public QWidget {
    Q_OBJECT
public:
    explicit FlightCard(const FlightData &data, QWidget *parent = nullptr);

signals:
    void bookClicked(QString flightId); // 点击预订按钮时发射信号

private:
    FlightData m_data;
    void setupUi(); // 初始化界面布局
};

#endif // FLIGHTCARD_H
