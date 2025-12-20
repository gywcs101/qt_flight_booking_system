#include "FlightCard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

FlightCard::FlightCard(const FlightData &data, QWidget *parent)
    : QWidget(parent), m_data(data) {
    setupUi();
    setFavoriteState(m_data.isFavorite);

    // 动画特效初始化
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(0.0);
    this->setGraphicsEffect(m_opacityEffect);
}

void FlightCard::startEntryAnimation(int delay) {
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

void FlightCard::setupUi() {
    this->setFixedHeight(125);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("FlightCard { background: white; border-radius: 8px; border: 1px solid #E0E0E0; }"
                        "FlightCard:hover { border: 1px solid #0078D7; }");

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
    col2->setAlignment(Qt::AlignCenter); // 居中对齐
    mainLayout->addLayout(col2, 2);

    // --- 中间：箭头 ---
    QLabel *arrow = new QLabel("──✈──");
    // 稍微上移一点，因为两边加了日期变高了
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
    col3->setAlignment(Qt::AlignCenter); // 居中对齐
    mainLayout->addLayout(col3, 2);

    // --- 第4列：价格 ---
    QVBoxLayout *col4 = new QVBoxLayout;
    QLabel *price = new QLabel(QString("¥%1").arg(m_data.price));
    price->setStyleSheet("color: #FF6600; font-weight: bold; font-size: 20px;");
    col4->addWidget(price);
    col4->setAlignment(Qt::AlignRight);

    // --- 第5列：按钮区 ---
    QVBoxLayout *btnLayout = new QVBoxLayout;
    btnLayout->setSpacing(5);

    QPushButton *btnBook = new QPushButton("预订");
    btnBook->setFixedSize(80, 30);
    btnBook->setStyleSheet("QPushButton { background-color: #0078D7; color: white; border-radius: 4px; border:none; font-weight: bold; }"
                           "QPushButton:hover { background-color: #005A9E; }");
    connect(btnBook, &QPushButton::clicked, [this](){ emit bookClicked(m_data.flightId); });

    m_btnFav = new QPushButton();
    m_btnFav->setFixedSize(80, 30);
    m_btnFav->setCursor(Qt::PointingHandCursor);

    connect(m_btnFav, &QPushButton::clicked, [this](){
        m_data.isFavorite = !m_data.isFavorite;
        setFavoriteState(m_data.isFavorite);
        emit favClicked(m_data.flightId, m_data.isFavorite);
    });

    btnLayout->addWidget(btnBook);
    btnLayout->addWidget(m_btnFav);
    btnLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addLayout(col4, 2);
    mainLayout->addLayout(btnLayout, 1);
}

void FlightCard::setFavoriteState(bool isFav) {
    m_data.isFavorite = isFav;
    if (isFav) {
        m_btnFav->setText("♥ 已收藏");
        m_btnFav->setStyleSheet("QPushButton { color: #FF4D4F; border: 1px solid #FF4D4F; background-color: #FFF1F0; border-radius: 4px; font-weight: bold;}");
    } else {
        m_btnFav->setText("♡ 收藏");
        m_btnFav->setStyleSheet("QPushButton { color: #606266; border: 1px solid #DCDFE6; background-color: white; border-radius: 4px; } QPushButton:hover { color: #409EFF; border-color: #409EFF; }");
    }
}
