#ifndef FLIGHTDATA_H
#define FLIGHTDATA_H

#include <QString>
#include <QDateTime>

struct FlightData {
    // === 基础信息 (对应数据库 flights 表) ===
    QString flightId;       // 航班号 (flight_id) VARCHAR
    QString airline;        // 航空公司 (airline)

    QString depCity;        // 出发城市 (departure_city)
    QString arrCity;        // 到达城市 (arrival_city)

    // 如果你的数据库里有机场名字段(如 departure_airport)，可以在这里加
    // 目前代码里我们通常用 depCity 代替显示

    QDateTime depTime;      // 出发时间 (departure_time)
    QDateTime arrTime;      // 到达时间 (arrival_time)

    double price;           // 票价 (price) -> 【关键】改签补差价、退票退款都需要它

    QString status;         // 状态 (status)

    int capacity;           // 容量 (capacity) - 可选
    int bookedCount;        // 已订数量 (booked_count) - 可选

    // === UI 辅助状态 (不直接存数据库，但用于界面显示) ===
    bool isFavorite = false; // 是否被当前用户收藏 (用于控制爱心红/灰)
};

#endif // FLIGHTDATA_H
