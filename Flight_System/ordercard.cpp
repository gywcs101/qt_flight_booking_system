#include "OrderCard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

OrderCard::OrderCard(const FlightData &data, int orderId, double paidPrice, QWidget *parent)
    : QWidget(parent), m_data(data), m_orderId(orderId), m_paidPrice(paidPrice) {
    setupUi();

    // =============================================================
    // 【关键步骤 1】初始化时设为全透明（隐藏状态，但占位）
    // =============================================================
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(0.0); // 0 = 完全透明
    this->setGraphicsEffect(m_opacityEffect);
}

// 【新增】实现入场动画
void OrderCard::startEntryAnimation(int delay)
{
    // 使用定时器实现延迟启动（制造瀑布流效果）
    QTimer::singleShot(delay, this, [=](){
        // 创建动画对象，目标是 opacity 属性
        QPropertyAnimation *anim = new QPropertyAnimation(m_opacityEffect, "opacity");
        anim->setDuration(500); // 动画持续 500ms
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::OutQuad); // 渐出曲线，比较柔和

        // 【优化】动画结束后移除特效
        // 原因：如果不移除 QGraphicsEffect，Windows 下文字抗锯齿会变差，导致字体看起来发虚
        connect(anim, &QPropertyAnimation::finished, [=](){
            this->setGraphicsEffect(nullptr);
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    });
}

// 完整的布局代码 (复用 FlightCard 样式)
void OrderCard::setupUi() {
    this->setFixedHeight(125);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("OrderCard { background: white; border-radius: 8px; border: 1px solid #E0E0E0; }"
                        "OrderCard:hover { border: 1px solid #0078D7; }");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 15, 20, 15);

    // 1. 航班号 + 航司
    QVBoxLayout *col1 = new QVBoxLayout;
    QLabel *lblId = new QLabel(m_data.flightId);
    lblId->setStyleSheet("font-weight: bold; color: #0078D7; font-size: 16px;");
    col1->addWidget(lblId);
    col1->addWidget(new QLabel(m_data.airline));
    mainLayout->addLayout(col1, 2);

    // 2. 出发时间 + 城市
    QVBoxLayout *col2 = new QVBoxLayout;
    QLabel *t1 = new QLabel(m_data.depTime.toString("HH:mm"));
    t1->setStyleSheet("font-weight: bold; font-size: 24px; color: #333;");
    col2->addWidget(t1);
    col2->addWidget(new QLabel(m_data.depCity));
    mainLayout->addLayout(col2, 2);

    // 3. 箭头
    mainLayout->addWidget(new QLabel("──✈──"), 1, Qt::AlignCenter);

    // 4. 到达时间 + 城市
    QVBoxLayout *col3 = new QVBoxLayout;
    QLabel *t2 = new QLabel(m_data.arrTime.toString("HH:mm"));
    t2->setStyleSheet("font-weight: bold; font-size: 24px; color: #333;");
    col3->addWidget(t2);
    col3->addWidget(new QLabel(m_data.arrCity));
    mainLayout->addLayout(col3, 2);

    // 5. 实付价格 + 状态
    QVBoxLayout *col4 = new QVBoxLayout;
    QLabel *price = new QLabel(QString("实付 ¥%1").arg(m_paidPrice));
    price->setStyleSheet("color: #FF6600; font-weight: bold; font-size: 18px;");
    QLabel *status = new QLabel("已支付");
    status->setStyleSheet("color: #67C23A; font-weight: bold; font-size: 12px;");
    col4->addWidget(price);
    col4->addWidget(status);
    col4->setAlignment(Qt::AlignRight);

    // 6. 按钮区 (改签 + 退票)
    QVBoxLayout *btnLayout = new QVBoxLayout;
    btnLayout->setSpacing(5);

    QPushButton *btnChange = new QPushButton("改签");
    btnChange->setFixedSize(80, 30);
    btnChange->setCursor(Qt::PointingHandCursor);
    btnChange->setStyleSheet("QPushButton { background-color: #0078D7; color: white; border-radius: 4px; border:none; font-weight: bold; }"
                             "QPushButton:hover { background-color: #005A9E; }");

    QPushButton *btnRefund = new QPushButton("退票");
    btnRefund->setFixedSize(80, 30);
    btnRefund->setCursor(Qt::PointingHandCursor);
    btnRefund->setStyleSheet("QPushButton { background-color: white; color: #666; border: 1px solid #DCDFE6; border-radius: 4px; }"
                             "QPushButton:hover { color: #F56C6C; border-color: #F56C6C; }");

    connect(btnChange, &QPushButton::clicked, [this](){
        emit changeClicked(m_orderId, m_data.flightId, m_data.depCity, m_data.arrCity, m_paidPrice);
    });
    connect(btnRefund, &QPushButton::clicked, [this](){
        emit refundClicked(m_orderId, m_paidPrice, m_data.flightId);
    });

    btnLayout->addWidget(btnChange);
    btnLayout->addWidget(btnRefund);
    btnLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addLayout(col4, 2);
    mainLayout->addLayout(btnLayout, 1);
}
