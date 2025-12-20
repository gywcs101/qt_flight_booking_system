#include "flightmapwidgets.h"
#include <QDebug>

CityDetailCard::CityDetailCard(QWidget *parent) : QWidget(parent) {
    // 设置卡片固定大小
    this->setFixedSize(200, 180);
    // 设置背景和阴影样式，或者用QSS
    // this->setStyleSheet("QWidget#CityDetailCard { background-color: white; border-radius: 8px; border: 1px solid #ccc; }");

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    // 1、图片
    imgLabel = new QLabel(this);
    imgLabel->setFixedHeight(120);
    imgLabel->setStyleSheet("border: top-left-radius: 8px; border-top-right-radius: 8px; background-color: #eee;");
    imgLabel->setAlignment(Qt::AlignCenter); // 图片居中
    imgLabel->setScaledContents(true); // 图片自适应

    // 2、文字
    infoLabel = new QLabel(this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-family: 'Microsoft Yahei'; font-size: 14px; padding: 16px; color: #333;");

    layout->addWidget(imgLabel);
    layout->addWidget(infoLabel);
}

// ======================= MapMarkerBtn 实现 =======================

MapMarkerBtn::MapMarkerBtn(const QString &city, int price, const QString &imgUrl, QWidget *parent)
    : QPushButton(parent), m_city(city), m_price(price), m_imgUrl(imgUrl)
{
    setFixedSize(70, 55); // 稍微加高一点，给三角形留位置
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_TranslucentBackground); // 设置透明背景
}

void MapMarkerBtn::paintEvent(QPaintEvent *event) {
    // ... (你的绘图代码保持不变，不需要修改) ...
    // 确保最后调用了父类的绘制或者自己画完了所有内容
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 颜色定义
    QColor bgColor(0, 134, 246); // 携程蓝
    QColor hoverColor(255, 153, 0); // 悬停橙色

    if (m_isHover) { // 使用成员变量判断
        painter.setBrush(hoverColor);
        painter.setPen(hoverColor);
    } else {
        painter.setBrush(bgColor);
        painter.setPen(bgColor);
    }

    // 绘制形状 (圆角矩形 + 底部三角形)
    int w = width();
    int h = height() - 8; // 留出8像素给三角形
    int r = 5; // 圆角半径

    QPainterPath path;
    // 上半部分圆角矩形
    path.addRoundedRect(0, 0, w, h, r, r);
    // 底部小三角形
    path.moveTo(w / 2 - 6, h);     // 三角形左点
    path.lineTo(w / 2, h + 6);     // 三角形尖端 (向下)
    path.lineTo(w / 2 + 6, h);     // 三角形右点
    painter.drawPath(path);

    // 绘制文字
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(12);
    font.setBold(true);
    painter.setFont(font);

    // 绘制城市名 (上半部)
    QRect topRect(0, 5, w, h/2 - 5);
    painter.drawText(topRect, Qt::AlignCenter, m_city);

    // 绘制价格 (下半部)
    QRect bottomRect(0, h/2, w, h/2 - 5);
    painter.drawText(bottomRect, Qt::AlignCenter, QString("¥%1").arg(m_price));
}

// ================== 【修改点 2：修复鼠标进入事件】 ==================
void MapMarkerBtn::enterEvent(QEnterEvent *event) {
    m_isHover = true;
    update(); // 触发重绘变色
    // 发送信号，注意：这里的 this->pos() 是按钮在代理控件内的坐标(通常是0,0)，
    // 实际坐标计算我们在外部处理，这里只需要发信号通知即可。
    emit hoverEntered(m_city, m_price, m_imgUrl, this->pos());
    QPushButton::enterEvent(event);
}

void MapMarkerBtn::leaveEvent(QEvent *event) {
    m_isHover = false;
    update(); // 触发重绘恢复颜色
    emit hoverLeft();
    QPushButton::leaveEvent(event);
}
