#include "OrderCard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

OrderCard::OrderCard(const FlightData &data, int orderId, double paidPrice, QWidget *parent)
    : QWidget(parent), m_data(data), m_orderId(orderId), m_paidPrice(paidPrice) {
    setupUi();

    // 动画特效初始化
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(0.0);
    this->setGraphicsEffect(m_opacityEffect);
}

void OrderCard::startEntryAnimation(int delay)
{
    QTimer::singleShot(delay, this, [=](){
        QPropertyAnimation *anim = new QPropertyAnimation(m_opacityEffect, "opacity");
        anim->setDuration(500);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::OutQuad);

        connect(anim, &QPropertyAnimation::finished, [=](){
            this->setGraphicsEffect(nullptr);
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    });
}

void OrderCard::setupUi() {
    this->setFixedHeight(125);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("OrderCard { background: white; border-radius: 8px; border: 1px solid #E0E0E0; }"
                        "OrderCard:hover { border: 1px solid #0078D7; }");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 15, 20, 15);

    // --- 第1列：航班号 + 航司 ---
    QVBoxLayout *col1 = new QVBoxLayout;
    QLabel *lblId = new QLabel(m_data.flightId);
    lblId->setStyleSheet("font-weight: bold; color: #0078D7; font-size: 16px;");
    col1->addWidget(lblId);
    col1->addWidget(new QLabel(m_data.airline));
    mainLayout->addLayout(col1, 2);

    // --- 第2列：出发时间 + 城市 + 日期 ---
    QVBoxLayout *col2 = new QVBoxLayout;
    QLabel *t1 = new QLabel(m_data.depTime.toString("HH:mm"));
    t1->setStyleSheet("font-weight: bold; font-size: 24px; color: #333;");

    QLabel *c1 = new QLabel(m_data.depCity);
    c1->setStyleSheet("color: #666; font-size: 14px;");

    // 【新增】出发日期
    QLabel *d1 = new QLabel(m_data.depTime.toString("MM-dd"));
    d1->setStyleSheet("color: #999; font-size: 12px;");

    col2->addWidget(t1);
    col2->addWidget(c1);
    col2->addWidget(d1);
    col2->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(col2, 2);

    // --- 中间：箭头 ---
    QLabel *arrow = new QLabel("──✈──");
    arrow->setStyleSheet("color: #DDD; font-size: 10px; margin-bottom: 20px;");
    mainLayout->addWidget(arrow, 1, Qt::AlignCenter);

    // --- 第3列：到达时间 + 城市 + 日期 ---
    QVBoxLayout *col3 = new QVBoxLayout;
    QLabel *t2 = new QLabel(m_data.arrTime.toString("HH:mm"));
    t2->setStyleSheet("font-weight: bold; font-size: 24px; color: #333;");

    QLabel *c2 = new QLabel(m_data.arrCity);
    c2->setStyleSheet("color: #666; font-size: 14px;");

    // 【新增】到达日期
    QLabel *d2 = new QLabel(m_data.arrTime.toString("MM-dd"));
    d2->setStyleSheet("color: #999; font-size: 12px;");

    col3->addWidget(t2);
    col3->addWidget(c2);
    col3->addWidget(d2);
    col3->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(col3, 2);

    // --- 第4列：实付价格 + 状态 ---
    QVBoxLayout *col4 = new QVBoxLayout;
    QLabel *price = new QLabel(QString("实付 ¥%1").arg(m_paidPrice));
    price->setStyleSheet("color: #FF6600; font-weight: bold; font-size: 18px;");

    QLabel *status = new QLabel("已支付");
    status->setStyleSheet("color: #67C23A; font-weight: bold; font-size: 12px;");

    col4->addWidget(price);
    col4->addWidget(status);
    col4->setAlignment(Qt::AlignRight);

    // --- 第5列：按钮区 (改签 + 退票) ---
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
