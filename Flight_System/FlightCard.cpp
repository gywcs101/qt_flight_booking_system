#include "FlightCard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

FlightCard::FlightCard(const FlightData &data, QWidget *parent)
    : QWidget(parent), m_data(data) {
    setupUi();
    setFavoriteState(m_data.isFavorite);
}

void FlightCard::setupUi() {
    // 【修改】增加高度以容纳两个大按钮
    this->setFixedHeight(125);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("FlightCard { background: white; border-radius: 8px; border: 1px solid #E0E0E0; }"
                        "FlightCard:hover { border: 1px solid #0078D7; }");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 15, 20, 15);

    // 1. 航班号
    QVBoxLayout *col1 = new QVBoxLayout;
    QLabel *lblId = new QLabel(m_data.flightId);
    lblId->setStyleSheet("font-weight: bold; color: #0078D7; font-size: 16px;");
    col1->addWidget(lblId);
    col1->addWidget(new QLabel(m_data.airline));
    mainLayout->addLayout(col1, 2);

    // 2. 出发
    QVBoxLayout *col2 = new QVBoxLayout;
    QLabel *t1 = new QLabel(m_data.depTime.toString("HH:mm"));
    t1->setStyleSheet("font-weight: bold; font-size: 24px;");
    col2->addWidget(t1);
    col2->addWidget(new QLabel(m_data.depCity));
    mainLayout->addLayout(col2, 2);

    // 3. 箭头
    mainLayout->addWidget(new QLabel("──✈──"), 1, Qt::AlignCenter);

    // 4. 到达
    QVBoxLayout *col3 = new QVBoxLayout;
    QLabel *t2 = new QLabel(m_data.arrTime.toString("HH:mm"));
    t2->setStyleSheet("font-weight: bold; font-size: 24px;");
    col3->addWidget(t2);
    col3->addWidget(new QLabel(m_data.arrCity));
    mainLayout->addLayout(col3, 2);

    // 5. 价格
    QVBoxLayout *col4 = new QVBoxLayout;
    QLabel *price = new QLabel(QString("¥%1").arg(m_data.price));
    price->setStyleSheet("color: #FF6600; font-weight: bold; font-size: 20px;");
    col4->addWidget(price);
    col4->setAlignment(Qt::AlignRight);

    // 6. 按钮区
    QVBoxLayout *btnLayout = new QVBoxLayout;
    btnLayout->setSpacing(5); // 按钮间距

    // 预订按钮
    QPushButton *btnBook = new QPushButton("预订");
    btnBook->setFixedSize(80, 30);
    btnBook->setStyleSheet("QPushButton { background-color: #0078D7; color: white; border-radius: 4px; border:none; font-weight: bold; }"
                           "QPushButton:hover { background-color: #005A9E; }");
    connect(btnBook, &QPushButton::clicked, [this](){ emit bookClicked(m_data.flightId); });

    // 【修改】收藏按钮：尺寸设为 80x30
    m_btnFav = new QPushButton();
    m_btnFav->setFixedSize(80, 30);
    m_btnFav->setCursor(Qt::PointingHandCursor);
    // 注意：这里不用 setFlat(true)，我们要自定义边框样式

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
    if (isFav) {
        // 已收藏：浅红背景，红字红边框
        m_btnFav->setText("♥ 已收藏");
        m_btnFav->setStyleSheet("QPushButton { "
                                "   color: #FF4D4F; "
                                "   border: 1px solid #FF4D4F; "
                                "   background-color: #FFF1F0; "
                                "   border-radius: 4px; font-weight: bold;"
                                "}");
    } else {
        // 未收藏：白底灰字
        m_btnFav->setText("♡ 收藏");
        m_btnFav->setStyleSheet("QPushButton { "
                                "   color: #606266; "
                                "   border: 1px solid #DCDFE6; "
                                "   background-color: white; "
                                "   border-radius: 4px;"
                                "}"
                                "QPushButton:hover { color: #409EFF; border-color: #409EFF; }");
    }
}
