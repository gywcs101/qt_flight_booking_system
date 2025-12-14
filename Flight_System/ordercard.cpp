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
    this->setFixedHeight(120);
    this->setStyleSheet("OrderCard { background: white; border-radius: 8px; border: 1px solid #E0E0E0; }"
                        "OrderCard:hover { border: 1px solid #0078D7; }");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 1. 航班基础信息 (复用 FlightCard 的逻辑，简化版)
    QVBoxLayout *infoLayout = new QVBoxLayout;

    // 第一行：航班号 + 航司
    QLabel *lblId = new QLabel(m_data.flightId + "  " + m_data.airline);
    lblId->setStyleSheet("font-weight: bold; color: #0078D7; font-size: 16px;");

    // 第二行：时间地点
    QString timeInfo = QString("%1 %2  ─────  %3 %4")
                           .arg(m_data.depTime.toString("HH:mm"), m_data.depCity)
                           .arg(m_data.arrTime.toString("HH:mm"), m_data.arrCity);
    QLabel *lblTime = new QLabel(timeInfo);
    lblTime->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");

    // 第三行：日期
    QLabel *lblDate = new QLabel(m_data.depTime.toString("yyyy-MM-dd"));
    lblDate->setStyleSheet("color: #666;");

    infoLayout->addWidget(lblId);
    infoLayout->addWidget(lblTime);
    infoLayout->addWidget(lblDate);

    // 2. 价格与状态
    QVBoxLayout *statusLayout = new QVBoxLayout;
    QLabel *priceLbl = new QLabel(QString("实付: ¥%1").arg(m_paidPrice));
    priceLbl->setStyleSheet("color: #FF6600; font-weight: bold; font-size: 16px;");

    QLabel *statusLbl = new QLabel("已支付");
    statusLbl->setStyleSheet("background-color: #E1F3D8; color: #67C23A; padding: 4px; border-radius: 4px;");
    statusLbl->setAlignment(Qt::AlignCenter);

    statusLayout->addWidget(priceLbl);
    statusLayout->addWidget(statusLbl);
    statusLayout->setAlignment(Qt::AlignRight);

    // 3. 按钮区 (改签 / 退票)
    QVBoxLayout *btnLayout = new QVBoxLayout;

    QPushButton *btnChange = new QPushButton("改签");
    btnChange->setFixedSize(80, 30);
    btnChange->setStyleSheet("QPushButton { background-color: #0078D7; color: white; border-radius: 4px; }"
                             "QPushButton:hover { background-color: #005A9E; }");

    QPushButton *btnRefund = new QPushButton("退票");
    btnRefund->setFixedSize(80, 30);
    btnRefund->setStyleSheet("QPushButton { background-color: white; color: #666; border: 1px solid #CCC; border-radius: 4px; }"
                             "QPushButton:hover { color: #F56C6C; border-color: #F56C6C; }");

    connect(btnRefund, &QPushButton::clicked, [this](){ emit refundClicked(m_orderId, m_paidPrice, m_data.flightId); });
    connect(btnChange, &QPushButton::clicked, [this](){ emit changeClicked(m_orderId, m_data.flightId, m_data.depCity, m_data.arrCity, m_paidPrice); });

    btnLayout->addWidget(btnChange);
    btnLayout->addWidget(btnRefund);

    mainLayout->addLayout(infoLayout, 3);
    mainLayout->addLayout(statusLayout, 1);
    mainLayout->addLayout(btnLayout, 1);
}
