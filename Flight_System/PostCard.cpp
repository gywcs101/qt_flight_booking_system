#include "PostCard.h"

PostCard::PostCard(const PostData &data, QWidget *parent)
    : QWidget(parent), m_data(data) // 将传入的数据保存到成员变量 m_data 中
{
    this->setFixedSize(220, 300);
    this->setCursor(Qt::PointingHandCursor);
    this->setStyleSheet("PostCard { background-color: white; border-radius: 8px; }"); // 加上白色背景看起来更好

    // 1. 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(5);

    // 2. 封面图 (使用传入的 data.imagePath)
    m_coverLabel = new QLabel(this);
    m_coverLabel->setScaledContents(true);
    QPixmap coverPix(data.imagePath);
    if(!coverPix.isNull()){
        // 保持比例裁剪显示，防止图片拉伸变形
        m_coverLabel->setPixmap(coverPix.scaled(220, 220, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        m_coverLabel->setText("无图片");
    }
    m_coverLabel->setStyleSheet("border-top-left-radius: 8px; border-top-right-radius: 8px;"); // 仅顶部圆角
    m_coverLabel->setFixedHeight(220);
    m_coverLabel->setAlignment(Qt::AlignCenter);

    // 3. 底部信息区域
    QWidget *bottomWidget = new QWidget(this);
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(8, 0, 8, 8); // 稍微调整边距

    // 标题 (使用 data.title)
    m_titleLabel = new QLabel(data.title, this);
    m_titleLabel->setWordWrap(true);
    // 限制标题最多显示两行，防止撑破布局
    m_titleLabel->setFixedHeight(40);
    m_titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #333;");
    m_titleLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // 作者行
    QWidget *authorWidget = new QWidget(this);
    QHBoxLayout *authorLayout = new QHBoxLayout(authorWidget);
    authorLayout->setContentsMargins(0,0,0,0);
    authorLayout->setSpacing(8);

    // 头像 (使用 data.avatarPath)
    m_avatarLabel = new QLabel(this);
    m_avatarLabel->setFixedSize(24, 24);
    m_avatarLabel->setScaledContents(true);
    QPixmap avatarPix(data.avatarPath);
    if(!avatarPix.isNull()) {
        m_avatarLabel->setPixmap(avatarPix);
    }
    m_avatarLabel->setStyleSheet("border-radius: 12px; border: 1px solid #eee; background: #f0f0f0;");

    // 作者名 (使用 data.authorName)
    m_nameLabel = new QLabel(data.authorName, this);
    m_nameLabel->setStyleSheet("color: #666; font-size: 12px;");

    authorLayout->addWidget(m_avatarLabel);
    authorLayout->addWidget(m_nameLabel);
    authorLayout->addStretch();

    bottomLayout->addWidget(m_titleLabel);
    bottomLayout->addWidget(authorWidget);

    mainLayout->addWidget(m_coverLabel);
    mainLayout->addWidget(bottomWidget);
}

void PostCard::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        // 关键点：点击时发送完整的 m_data，这样详情页才有数据
        emit cardClicked(m_data);
    }
    QWidget::mousePressEvent(event);
}
