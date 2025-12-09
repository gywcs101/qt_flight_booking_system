#include "FlightCard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

FlightCard::FlightCard(const FlightData &data, QWidget *parent)
    : QWidget(parent), m_data(data) {
    setupUi();
}

void FlightCard::setupUi() {
    // 1. 设置卡片整体样式
    this->setFixedHeight(110);
    this->setAttribute(Qt::WA_StyledBackground, true); // 确保QSS生效
    this->setStyleSheet(
        "FlightCard { background-color: white; border-radius: 8px; border: 1px solid #E0E0E0; }"
        "FlightCard:hover { border: 1px solid #0078D7; }" // 悬停变蓝
        );

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 15, 20, 15);
    mainLayout->setSpacing(10);

    // --- 第1列：航班号与航司 ---
    QVBoxLayout *col1 = new QVBoxLayout;
    QLabel *lblId = new QLabel(m_data.flightId);
    lblId->setStyleSheet("font-weight: bold; color: #0078D7; font-size: 16px;");
    QLabel *lblAirline = new QLabel(m_data.airline);
    lblAirline->setStyleSheet("color: #666; font-size: 13px;");
    col1->addWidget(lblId);
    col1->addWidget(lblAirline);

    // --- 第2列：出发信息 ---
    QVBoxLayout *col2 = new QVBoxLayout;
    QLabel *lblDepTime = new QLabel(m_data.depTime.toString("HH:mm"));
    lblDepTime->setStyleSheet("font-weight: bold; font-size: 24px; color: #333;");
    QLabel *lblDepInfo = new QLabel(m_data.depCity + " " + m_data.depTime.toString("MM-dd"));
    lblDepInfo->setStyleSheet("color: #888; font-size: 12px;");
    col2->addWidget(lblDepTime);
    col2->addWidget(lblDepInfo);
    col2->setAlignment(Qt::AlignCenter);

    // --- 第3列：箭头装饰 ---
    QLabel *arrow = new QLabel("──────✈");
    arrow->setStyleSheet("color: #DDD; font-size: 10px;");
    arrow->setAlignment(Qt::AlignCenter);

    // --- 第4列：到达信息 ---
    QVBoxLayout *col3 = new QVBoxLayout;
    QLabel *lblArrTime = new QLabel(m_data.arrTime.toString("HH:mm"));
    lblArrTime->setStyleSheet("font-weight: bold; font-size: 24px; color: #333;");
    QLabel *lblArrInfo = new QLabel(m_data.arrCity + " " + m_data.arrTime.toString("MM-dd"));
    lblArrInfo->setStyleSheet("color: #888; font-size: 12px;");
    col3->addWidget(lblArrTime);
    col3->addWidget(lblArrInfo);
    col3->setAlignment(Qt::AlignCenter);

    // --- 第5列：价格、状态、按钮 ---
    QVBoxLayout *col4 = new QVBoxLayout;

    // 价格
    QLabel *lblPrice = new QLabel(QString("¥%1").arg(m_data.price));
    lblPrice->setStyleSheet("color: #FF6600; font-weight: bold; font-size: 20px;");
    lblPrice->setAlignment(Qt::AlignRight);

    // 状态标签
    QLabel *lblStatus = new QLabel(m_data.status);
    lblStatus->setAlignment(Qt::AlignRight);
    if (m_data.status == "延误") {
        lblStatus->setStyleSheet("color: red; font-weight: bold; font-size: 12px;");
    } else {
        lblStatus->setStyleSheet("color: #28a745; font-size: 12px;");
    }

    col4->addWidget(lblPrice);
    col4->addWidget(lblStatus);

    // 预订按钮
    QVBoxLayout *colBtn = new QVBoxLayout;
    QPushButton *btnBook = new QPushButton("预订");
    btnBook->setFixedSize(80, 30);
    btnBook->setCursor(Qt::PointingHandCursor);
    btnBook->setStyleSheet(
        "QPushButton { background-color: #0078D7; color: white; border-radius: 4px; border: none; }"
        "QPushButton:hover { background-color: #005A9E; }"
        );
    connect(btnBook, &QPushButton::clicked, [this]() {
        emit bookClicked(m_data.flightId);
    });
    colBtn->addWidget(btnBook);
    colBtn->setAlignment(Qt::AlignCenter);

    // 添加到主布局
    mainLayout->addLayout(col1, 2);
    mainLayout->addLayout(col2, 2);
    mainLayout->addWidget(arrow, 1);
    mainLayout->addLayout(col3, 2);
    mainLayout->addLayout(col4, 2);
    mainLayout->addLayout(colBtn, 1);
}
