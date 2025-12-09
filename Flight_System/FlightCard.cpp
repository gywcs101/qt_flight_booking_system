#include "FlightCard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

FlightCard::FlightCard(const FlightData &data, QWidget *parent)
    : QWidget(parent), m_data(data) {
    setupUi();
    // 初始化时，根据数据设置按钮状态
    setFavoriteState(m_data.isFavorite);
}

void FlightCard::setupUi() {
    // 1. 设置卡片整体样式
    this->setFixedHeight(110);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(
        "FlightCard { background-color: white; border-radius: 8px; border: 1px solid #E0E0E0; }"
        "FlightCard:hover { border: 1px solid #0078D7; }"
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

    // --- 第5列：价格、状态 ---
    QVBoxLayout *col4 = new QVBoxLayout;
    QLabel *lblPrice = new QLabel(QString("¥%1").arg(m_data.price));
    lblPrice->setStyleSheet("color: #FF6600; font-weight: bold; font-size: 20px;");
    lblPrice->setAlignment(Qt::AlignRight);
    QLabel *lblStatus = new QLabel(m_data.status);
    lblStatus->setAlignment(Qt::AlignRight);
    if (m_data.status == "延误") {
        lblStatus->setStyleSheet("color: red; font-size: 12px;");
    } else {
        lblStatus->setStyleSheet("color: #28a745; font-size: 12px;");
    }
    col4->addWidget(lblPrice);
    col4->addWidget(lblStatus);

    // --- 第6列：操作按钮区 (预订 + 收藏) ---
    QVBoxLayout *colBtn = new QVBoxLayout;

    // 预订按钮
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

    // 【新增】收藏按钮
    m_btnFav = new QPushButton();
    m_btnFav->setFixedSize(80, 25);
    m_btnFav->setCursor(Qt::PointingHandCursor);
    m_btnFav->setFlat(true); // 扁平化，没有边框

    connect(m_btnFav, &QPushButton::clicked, [this]() {
        // 1. 状态取反
        m_data.isFavorite = !m_data.isFavorite;
        // 2. 更新UI颜色
        setFavoriteState(m_data.isFavorite);
        // 3. 发送信号给主界面去改数据库
        emit favClicked(m_data.flightId, m_data.isFavorite);
    });

    colBtn->addWidget(btnBook);
    colBtn->addWidget(m_btnFav); // 把收藏按钮放在预订按钮下面
    colBtn->setAlignment(Qt::AlignCenter);

    // 添加到主布局
    mainLayout->addLayout(col1, 2);
    mainLayout->addLayout(col2, 2);
    mainLayout->addWidget(arrow, 1);
    mainLayout->addLayout(col3, 2);
    mainLayout->addLayout(col4, 2);
    mainLayout->addLayout(colBtn, 1);
}

// 【新增】改变按钮外观
void FlightCard::setFavoriteState(bool isFav) {
    if (isFav) {
        // 实心红心
        m_btnFav->setText("♥ 已收藏");
        m_btnFav->setStyleSheet("color: #FF4D4F; font-size: 13px; font-weight: bold; border: none; background: transparent;");
    } else {
        // 空心灰心
        m_btnFav->setText("♡ 收藏");
        m_btnFav->setStyleSheet("color: #909399; font-size: 13px; border: none; background: transparent;");
        // 鼠标悬停变色提示
        // 可以在QSS里加 QPushButton:hover { color: #FF4D4F; }
    }
}
