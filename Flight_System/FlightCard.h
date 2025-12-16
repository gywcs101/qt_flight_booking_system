#ifndef FLIGHTCARD_H
#define FLIGHTCARD_H

#include <QWidget>
#include "FlightData.h"

// 【新增】动画所需头文件
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>

class QPushButton;

class FlightCard : public QWidget {
    Q_OBJECT
public:
    explicit FlightCard(const FlightData &data, QWidget *parent = nullptr);
    void setFavoriteState(bool isFav);

    // 【新增】启动入场动画
    void startEntryAnimation(int delay);

signals:
    void bookClicked(QString flightId);
    void favClicked(QString flightId, bool isFavorite);

private:
    FlightData m_data;
    QPushButton *m_btnFav;

    // 【新增】透明度特效指针
    QGraphicsOpacityEffect* m_opacityEffect;

    void setupUi();
};

#endif // FLIGHTCARD_H
