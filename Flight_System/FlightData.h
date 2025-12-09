#ifndef FLIGHTDATA_H
#define FLIGHTDATA_H

#include <QString>
#include <QDateTime>

struct FlightData {
    QString flightId;       // 对应数据库的 VARCHAR flight_id
    QString airline;
    QString depCity;
    QString arrCity;
    QDateTime depTime;
    QDateTime arrTime;
    double price;
    QString status;

    // 是否被当前用户收藏
    bool isFavorite = false;
};

#endif // FLIGHTDATA_H
