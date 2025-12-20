#ifndef FLIGHTMAPWIDGETS_H
#define FLIGHTMAPWIDGETS_H

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QEnterEvent>

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
    void leaveEvent(QEvent *event) override;      // 鼠标离开
    void paintEvent(QPaintEvent *event) override; // 重绘按钮

<<<<<<< HEAD
signals:
    void hoverEntered(QString city, int price, QString imgUrl, QPoint localPos);
    void hoverLeft();

=======
    // ================== 【修改点 1：添加信号声明】 ==================
signals:
    // 声明鼠标进入和离开的信号，供外部连接
    void hoverEntered(QString city, int price, QString imgUrl, QPoint localPos);
    void hoverLeft();
    // ==============================================================
>>>>>>> parent of fa23cea (Revert "Merge branch 'main' of https://github.com/gywcs101/qt_flight_booking_system")

private:
    QString m_city;     // 城市名
    int m_price;        // 价格
    QString m_imgUrl;   // 图片路径
    bool m_isHover = false; // 标记是否处于悬停状态
};

#endif // FLIGHTMAPWIDGETS_H
