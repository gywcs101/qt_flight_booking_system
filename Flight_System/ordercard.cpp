#include "OrderCard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

OrderCard::OrderCard(const FlightData &data, int orderId, double paidPrice, QWidget *parent)
    : QWidget(parent), m_data(data), m_orderId(orderId), m_paidPrice(paidPrice) {
    setupUi();
}

void OrderCard::setupUi() {
    // 1. 【样式统一】设置卡片高度、背景色、边框、圆角
    this->setFixedHeight(125); // 和 FlightCard 高度保持一致
    this->setAttribute(Qt::WA_StyledBackground, true);

    // 注意：这里选择器用 OrderCard，确保样式应用到当前类
    this->setStyleSheet("OrderCard { background: white; border-radius: 8px; border: 1px solid #E0E0E0; }"
                        "OrderCard:hover { border: 1px solid #0078D7; }"); // 悬停变蓝效果

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 15, 20, 15); // 内边距保持一致

    // ==========================================
    // 2. 左侧信息区 (直接复用 FlightCard 的布局)
    // ==========================================

    // --- 第1列：航班号 + 航司 ---
    QVBoxLayout *col1 = new QVBoxLayout;
    QLabel *lblId = new QLabel(m_data.flightId);
    lblId->setStyleSheet("font-weight: bold; color: #0078D7; font-size: 16px;");
    QLabel *lblAirline = new QLabel(m_data.airline);
    lblAirline->setStyleSheet("color: #666; font-size: 13px;");
    col1->addWidget(lblId);
    col1->addWidget(lblAirline);
    mainLayout->addLayout(col1, 2);

    // --- 第2列：出发时间 + 城市 ---
    QVBoxLayout *col2 = new QVBoxLayout;
    QLabel *t1 = new QLabel(m_data.depTime.toString("HH:mm"));
    t1->setStyleSheet("font-weight: bold; font-size: 24px; color: #333;");
    QLabel *c1 = new QLabel(m_data.depCity);
    c1->setStyleSheet("color: #888; font-size: 12px;");
    col2->addWidget(t1);
    col2->addWidget(c1);
    mainLayout->addLayout(col2, 2);

    // --- 中间：箭头装饰 ---
    QLabel *arrow = new QLabel("──✈──");
    arrow->setStyleSheet("color: #DDD; font-size: 10px;");
    mainLayout->addWidget(arrow, 1, Qt::AlignCenter);

    // --- 第3列：到达时间 + 城市 ---
    QVBoxLayout *col3 = new QVBoxLayout;
    QLabel *t2 = new QLabel(m_data.arrTime.toString("HH:mm"));
    t2->setStyleSheet("font-weight: bold; font-size: 24px; color: #333;");
    QLabel *c2 = new QLabel(m_data.arrCity);
    c2->setStyleSheet("color: #888; font-size: 12px;");
    col3->addWidget(t2);
    col3->addWidget(c2);
    mainLayout->addLayout(col3, 2);

    // ==========================================
    // 3. 右侧差异区 (价格显示实付，按钮改为改签/退票)
    // ==========================================

    // --- 第4列：实付价格 + 状态 ---
    QVBoxLayout *col4 = new QVBoxLayout;
    QLabel *price = new QLabel(QString("¥%1").arg(m_paidPrice)); // 显示实付价格
    price->setStyleSheet("color: #FF6600; font-weight: bold; font-size: 20px;");

    // 状态标签
    QLabel *status = new QLabel("已支付");
    status->setStyleSheet("color: #67C23A; font-weight: bold; font-size: 12px;"); // 绿色文字

    col4->addWidget(price);
    col4->addWidget(status);
    col4->setAlignment(Qt::AlignRight);

    // --- 第5列：操作按钮 (改签 + 退票) ---
    QVBoxLayout *btnLayout = new QVBoxLayout;
    btnLayout->setSpacing(5); // 按钮间距

    // 按钮 1：改签 (蓝色，主要操作)
    QPushButton *btnChange = new QPushButton("改签");
    btnChange->setFixedSize(80, 30);
    btnChange->setCursor(Qt::PointingHandCursor);
    btnChange->setStyleSheet("QPushButton { background-color: #0078D7; color: white; border-radius: 4px; border:none; font-weight: bold; }"
                             "QPushButton:hover { background-color: #005A9E; }");

    // 按钮 2：退票 (白色带边框，次要操作/危险操作)
    QPushButton *btnRefund = new QPushButton("退票");
    btnRefund->setFixedSize(80, 30);
    btnRefund->setCursor(Qt::PointingHandCursor);
    btnRefund->setStyleSheet("QPushButton { background-color: white; color: #666; border: 1px solid #DCDFE6; border-radius: 4px; }"
                             "QPushButton:hover { color: #F56C6C; border-color: #F56C6C; }"); // 悬停变红

    // 连接信号
    connect(btnRefund, &QPushButton::clicked, [this](){
        emit refundClicked(m_orderId, m_paidPrice, m_data.flightId);
    });
    connect(btnChange, &QPushButton::clicked, [this](){
        emit changeClicked(m_orderId, m_data.flightId, m_data.depCity, m_data.arrCity, m_paidPrice);
    });

    btnLayout->addWidget(btnChange);
    btnLayout->addWidget(btnRefund);
    btnLayout->setAlignment(Qt::AlignCenter);

    // 将列加入主布局
    mainLayout->addLayout(col4, 2);
    mainLayout->addLayout(btnLayout, 1);
}
