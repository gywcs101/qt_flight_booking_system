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
    this->setFixedHeight(125);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("FlightCard { background: white; border-radius: 8px; border: 1px solid #E0E0E0; }"
                        "FlightCard:hover { border: 1px solid #0078D7; }");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 15, 20, 15);

    // ... (中间布局代码省略，保持你原来的) ...
    // 这里是你之前写的布局代码，不用动，只需要检查 col1, col2... 是否都 addLayout 进去了
    // 为了节省篇幅我只写按钮部分，请保留你原来的中间代码

    // 假设这是前面的代码...
    QVBoxLayout *col1 = new QVBoxLayout;
    QLabel *lblId = new QLabel(m_data.flightId);
    lblId->setStyleSheet("font-weight: bold; color: #0078D7; font-size: 16px;");
    col1->addWidget(lblId);
    col1->addWidget(new QLabel(m_data.airline));
    mainLayout->addLayout(col1, 2);

    QVBoxLayout *col2 = new QVBoxLayout;
    QLabel *t1 = new QLabel(m_data.depTime.toString("HH:mm"));
    t1->setStyleSheet("font-weight: bold; font-size: 24px;");
    col2->addWidget(t1);
    col2->addWidget(new QLabel(m_data.depCity));
    mainLayout->addLayout(col2, 2);

    mainLayout->addWidget(new QLabel("──✈──"), 1, Qt::AlignCenter);

    QVBoxLayout *col3 = new QVBoxLayout;
    QLabel *t2 = new QLabel(m_data.arrTime.toString("HH:mm"));
    t2->setStyleSheet("font-weight: bold; font-size: 24px;");
    col3->addWidget(t2);
    col3->addWidget(new QLabel(m_data.arrCity));
    mainLayout->addLayout(col3, 2);

    QVBoxLayout *col4 = new QVBoxLayout;
    QLabel *price = new QLabel(QString("¥%1").arg(m_data.price));
    price->setStyleSheet("color: #FF6600; font-weight: bold; font-size: 20px;");
    col4->addWidget(price);
    col4->setAlignment(Qt::AlignRight);

    // 按钮区
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
    m_data.isFavorite = isFav; // 同步数据状态
    if (isFav) {
        m_btnFav->setText("♥ 已收藏");
        m_btnFav->setStyleSheet("QPushButton { color: #FF4D4F; border: 1px solid #FF4D4F; background-color: #FFF1F0; border-radius: 4px; font-weight: bold;}");
    } else {
        m_btnFav->setText("♡ 收藏");
        m_btnFav->setStyleSheet("QPushButton { color: #606266; border: 1px solid #DCDFE6; background-color: white; border-radius: 4px; } QPushButton:hover { color: #409EFF; border-color: #409EFF; }");
    }
}
