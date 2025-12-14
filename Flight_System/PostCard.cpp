#include "PostCard.h"

PostCard::PostCard(int id, QString title, QString author, QString coverPath, QString avatarPath, QWidget *parent)
    : QWidget(parent), m_id(id)
{
    this->setFixedSize(220, 300); // 设定卡片固定大小
    this->setCursor(Qt::PointingHandCursor); // 鼠标移上去变手型

    // 1. 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(5);

    // 2. 封面图 (上半部分)
    m_coverLabel = new QLabel(this);
    m_coverLabel->setScaledContents(true);
    m_coverLabel->setPixmap(QPixmap(coverPath));
    // 设置圆角效果
    m_coverLabel->setStyleSheet("border-radius: 8px;");
    m_coverLabel->setFixedHeight(220); // 封面图高度

    // 3. 底部信息区域 (头像+名字+标题)
    QWidget *bottomWidget = new QWidget(this);
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(5,0,5,5);

    // 标题
    m_titleLabel = new QLabel(title, this);
    m_titleLabel->setWordWrap(true); // 自动换行
    m_titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    // 作者行 (头像 + 名字)
    QWidget *authorWidget = new QWidget(this);
    QHBoxLayout *authorLayout = new QHBoxLayout(authorWidget);
    authorLayout->setContentsMargins(0,0,0,0);

    m_avatarLabel = new QLabel(this);
    m_avatarLabel->setFixedSize(24, 24);
    m_avatarLabel->setScaledContents(true);
    m_avatarLabel->setPixmap(QPixmap(avatarPath));
    // 圆形头像的关键样式
    m_avatarLabel->setStyleSheet("border-radius: 12px; border: 1px solid #ddd; background: white;");

    m_nameLabel = new QLabel(author, this);
    m_nameLabel->setStyleSheet("color: gray; font-size: 12px;");

    authorLayout->addWidget(m_avatarLabel);
    authorLayout->addWidget(m_nameLabel);
    authorLayout->addStretch();

    bottomLayout->addWidget(m_titleLabel);
    bottomLayout->addWidget(authorWidget);

    // 4. 组装
    mainLayout->addWidget(m_coverLabel);
    mainLayout->addWidget(bottomWidget);
}

void PostCard::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        emit cardClicked(m_id);
    }
}
