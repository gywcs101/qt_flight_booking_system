#ifndef FLIGHTCARD_H
#define FLIGHTCARD_H

#include <QWidget>
#include "FlightData.h"

class QPushButton; // 前置声明，减少编译依赖

class FlightCard : public QWidget {
    Q_OBJECT
public:
    explicit FlightCard(const FlightData &data, QWidget *parent = nullptr);

    // 【新增】设置收藏状态（改变爱心颜色）
    void setFavoriteState(bool isFav);

signals:
    void bookClicked(QString flightId);

    // 【新增】当点击收藏按钮时发出信号
    // 参数1: 航班号, 参数2: 点击后变成的状态(true=已收藏, false=未收藏)
    void favClicked(QString flightId, bool isFavorite);

private:
    FlightData m_data;
    QPushButton *m_btnFav; // 收藏按钮指针
    void setupUi();
};

#endif // FLIGHTCARD_H
