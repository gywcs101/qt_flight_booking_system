#ifndef FLIGHTDATA_H
#define FLIGHTDATA_H

#include <QString>
#include <QDateTime>

struct FlightData {
    QString flightId;       // 航班号 (flight_id)
    QString airline;        // 航空公司 (airline)
    QString depCity;        // 出发城市 (departure_city)
    QString arrCity;        // 到达城市 (arrival_city)
    QDateTime depTime;      // 出发时间 (departure_time)
    QDateTime arrTime;      // 到达时间 (arrival_time)
    double price;           // 价格 (price)
    int capacity;           // 容量 (capacity)
    int bookedCount;        // 已订 (booked_count)
    QString status;         // 状态 (status: 计划中/延误)
};

#endif // FLIGHTDATA_H
