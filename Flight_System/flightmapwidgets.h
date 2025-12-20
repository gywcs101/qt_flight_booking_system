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
    explicit MapMarkerBtn(const QString &city, int price, const QString &imgUrl, QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *event) override; // 鼠标进入
    void leaveEvent(QEvent *event) override;     // 鼠标离开
    void paintEvent(QPaintEvent *event) override; // 重绘按钮

signals:
    void hoverEntered(QString city, int price, QString imgUrl, QPoint localPos);
    void hoverLeft();


private:
    QString m_city;    // 城市名
    int m_price;       // 价格
    QString m_imgUrl;  // 图片路径
    bool m_isHover = false; // 标记是否处于悬停状态
};

#endif // FLIGHTMAPWIDGETS_H
