#include "flightmapwidgets.h"
#include <QDebug>

// ================= CityDetailCard 实现 =================

CityDetailCard::CityDetailCard(QWidget *parent) : QWidget(parent) {
    // 设置卡片固定大小
    this->setFixedSize(220, 180);
    // 设置白色背景和阴影效果需要在这里做，或者用QSS
    this->setStyleSheet("CityDetailCard { background-color: white; border-radius: 8px; border: 1px solid #ccc; }");

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    // 1. 图片区域
    imgLabel = new QLabel(this);
    imgLabel->setFixedHeight(120);
    imgLabel->setStyleSheet("border-top-left-radius: 8px; border-top-right-radius: 8px; background-color: #eee;");
    imgLabel->setAlignment(Qt::AlignCenter);
    imgLabel->setScaledContents(true); // 图片自适应

    // 2. 文字区域
    infoLabel = new QLabel(this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 14px; padding: 10px; color: #333;");

    layout->addWidget(imgLabel);
    layout->addWidget(infoLabel);
}

void CityDetailCard::setContent(const QString &city, int price, const QString &imagePath) {
    // 加载图片 (如果没有图片显示默认文字)
    QPixmap pix(imagePath);
    if(pix.isNull()) {
        imgLabel->setText("暂无图片");
    } else {
        imgLabel->setPixmap(pix);
    }

    // 设置富文本显示城市和价格
    infoLabel->setText(QString("<b>%1</b> &nbsp;&nbsp; <span style='color:#0086F6; font-size:16px;'>¥%2起</span>").arg(city).arg(price));
}

// ================= MapMarkerBtn 实现 =================

MapMarkerBtn::MapMarkerBtn(const QString &city, int price, const QString &imgUrl, QWidget *parent)
    : QPushButton(parent), m_city(city), m_price(price), m_imgUrl(imgUrl)
{
    setFixedSize(70, 55); // 稍微加高一点，给三角形留位置
    setCursor(Qt::PointingHandCursor);
    // 设置透明背景，因为我们要自己画背景
    setAttribute(Qt::WA_TranslucentBackground);
}

void MapMarkerBtn::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    // 1. 定义颜色
    QColor bgColor(0, 134, 246); // 携程蓝
    QColor hoverColor(255, 153, 0); // 悬停橙色

    if (underMouse()) {
        painter.setBrush(hoverColor);
        painter.setPen(hoverColor);
    } else {
        painter.setBrush(bgColor);
        painter.setPen(bgColor);
    }

    // 2. 绘制形状（圆角矩形 + 底部三角形）
    int w = width();
    int h = height() - 8; // 留出8像素给三角形
    int r = 5; // 圆角半径

    QPainterPath path;
    // 上半部分圆角矩形
    path.addRoundedRect(0, 0, w, h, r, r);

    // 底部小三角
    path.moveTo(w / 2 - 6, h); // 三角形左点
    path.lineTo(w / 2, h + 6); // 三角形尖端（向下）
    path.lineTo(w / 2 + 6, h); // 三角形右点

    painter.drawPath(path);

    // 3. 绘制文字
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(12);
    font.setBold(true);
    painter.setFont(font);

    // 绘制城市名（上半部）
    QRect topRect(0, 5, w, h/2 - 5);
    painter.drawText(topRect, Qt::AlignCenter, m_city);

    // 绘制价格（下半部）
    QRect bottomRect(0, h/2, w, h/2 - 5);
    painter.drawText(bottomRect, Qt::AlignCenter, QString("¥%1").arg(m_price));
}

// 【修改点 2】参数类型改为 QEnterEvent
void MapMarkerBtn::enterEvent(QEnterEvent *event) {
    // 当鼠标进入，发送信号，并把自己的位置传出去
    // mapToGlobal(QPoint(0,0)) 获取当前按钮在屏幕上的绝对位置
    emit hoverEntered(m_city, m_price, m_imgUrl, this->mapToGlobal(QPoint(0, 0)));

    // 【修改点 3】调用父类时，传入正确的 event 类型
    QPushButton::enterEvent(event);

    update(); // 触发重绘变色
}

void MapMarkerBtn::leaveEvent(QEvent *event) {
    emit hoverLeft();
    QPushButton::leaveEvent(event);
    update();
}
