#ifndef FLIGHTDATA_H
#define FLIGHTDATA_H

#include <QString>
#include <QDateTime>

struct FlightData {
    QString flightId;       // 航班号 (flight_id)
    QString airline;        // 航空公司
    QString depCity;        // 出发城市
    QString arrCity;        // 到达城市
    QDateTime depTime;      // 出发时间
    QDateTime arrTime;      // 到达时间
    double price;           // 价格
    int capacity;           // 容量
    int bookedCount;        // 已订
    QString status;         // 状态

    // 【新增】是否已收藏
    bool isFavorite = false;
};

#endif // FLIGHTDATA_H
