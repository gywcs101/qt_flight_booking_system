#ifndef FLIGHTCARD_H
#define FLIGHTCARD_H

#include <QWidget>
#include "FlightData.h"

class QPushButton;

class FlightCard : public QWidget {
    Q_OBJECT
public:
    explicit FlightCard(const FlightData &data, QWidget *parent = nullptr);
    void setFavoriteState(bool isFav);

signals:
    void bookClicked(QString flightId);
    void favClicked(QString flightId, bool isFavorite);

private:
    FlightData m_data;
    QPushButton *m_btnFav;
    void setupUi();
};

#endif // FLIGHTCARD_H
