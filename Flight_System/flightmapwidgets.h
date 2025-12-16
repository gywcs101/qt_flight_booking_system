#ifndef FLIGHTMAPWIDGETS_H
#define FLIGHTMAPWIDGETS_H

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsEffect>
#include <QEvent>       // 【新增】
#include <QEnterEvent>  // 【新增】针对 Qt 6

// --- 类1：详情展示卡片 ---
class CityDetailCard : public QWidget {
    Q_OBJECT
public:
    explicit CityDetailCard(QWidget *parent = nullptr);
    void setContent(const QString &city, int price, const QString &imagePath);

private:
    QLabel *imgLabel;
    QLabel *infoLabel;
};

// --- 类2：地图标记按钮 ---
class MapMarkerBtn : public QPushButton {
    Q_OBJECT
public:
    MapMarkerBtn(const QString &city, int price, const QString &imgUrl, QWidget *parent = nullptr);

signals:
    void hoverEntered(QString city, int price, QString imgUrl, QPoint globalPos);
    void hoverLeft();

protected:
    void paintEvent(QPaintEvent *event) override;

    // 【修改点 1】注意这里参数变成了 QEnterEvent
    void enterEvent(QEnterEvent *event) override;

    // 【注意】leaveEvent 依然是用 QEvent，不需要改
    void leaveEvent(QEvent *event) override;

private:
    QString m_city;
    int m_price;
    QString m_imgUrl;
};

#endif // FLIGHTMAPWIDGETS_H
