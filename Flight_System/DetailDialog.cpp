#include "DetailDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QPixmap>

DetailDialog::DetailDialog(const PostData &data, QWidget *parent) : QDialog(parent) {
    setupUi(data);
}

void DetailDialog::setupUi(const PostData &data) {
    // 1. 设置窗口基本属性
    this->setFixedSize(900, 700); // 窗口大小，可根据需要调整
    this->setWindowTitle(data.title);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); // 无边框窗口更美观，或者去掉 Frameless 使用默认
    this->setStyleSheet("QDialog { background-color: white; border-radius: 10px; border: 1px solid #ddd; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 2. 顶部导航栏（包含关闭按钮）
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(50);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);

    QLabel *titleLbl = new QLabel("详情", this);
    titleLbl->setStyleSheet("font-size: 16px; font-weight: bold; color: #333; margin-left: 10px;");

    QPushButton *closeBtn = new QPushButton("×", this);
    closeBtn->setFixedSize(40, 40);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setStyleSheet("QPushButton { font-size: 24px; border: none; background: transparent; color: #666; } QPushButton:hover { color: #000; }");
    connect(closeBtn, &QPushButton::clicked, this, &DetailDialog::accept);

    headerLayout->addWidget(titleLbl);
    headerLayout->addStretch();
    headerLayout->addWidget(closeBtn);

    mainLayout->addWidget(headerWidget);

    // 3. 滚动区域（防止内容过长）
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *contentContainer = new QWidget();
    contentContainer->setStyleSheet("background-color: white;");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentContainer);
    contentLayout->setContentsMargins(30, 10, 30, 30);
    contentLayout->setSpacing(20);

    // --- 内容部分 ---

    // A. 大图
    QLabel *bigImage = new QLabel(this);
    bigImage->setScaledContents(true);
    bigImage->setFixedHeight(500); // 图片高度固定，或者根据比例计算
    bigImage->setStyleSheet("border-radius: 8px;");
    QPixmap pix(data.imagePath);
    if(!pix.isNull()) {
        // 保持比例填充
        bigImage->setPixmap(pix.scaled(840, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        bigImage->setAlignment(Qt::AlignCenter);
    } else {
        bigImage->setText("图片加载失败");
        bigImage->setStyleSheet("background-color: #f0f0f0; color: #999; border-radius: 8px;");
        bigImage->setAlignment(Qt::AlignCenter);
    }

    // B. 作者栏
    QWidget *authorWidget = new QWidget(this);
    QHBoxLayout *authorLayout = new QHBoxLayout(authorWidget);
    authorLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *avatar = new QLabel(this);
    avatar->setFixedSize(40, 40);
    avatar->setStyleSheet("border-radius: 20px; background-color: #eee; border: 1px solid #eee;");
    avatar->setScaledContents(true);
    QPixmap avPix(data.avatarPath);
    if(!avPix.isNull()) avatar->setPixmap(avPix);

    QLabel *authorName = new QLabel(data.authorName, this);
    authorName->setStyleSheet("font-size: 16px; font-weight: bold; color: #333; margin-left: 8px;");

    authorLayout->addWidget(avatar);
    authorLayout->addWidget(authorName);
    authorLayout->addStretch();

    // C. 标题
    QLabel *contentTitle = new QLabel(data.title, this);
    contentTitle->setWordWrap(true);
    contentTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: #000; margin-top: 10px;");

    // D. 正文内容
    QLabel *contentText = new QLabel(data.content, this);
    contentText->setWordWrap(true);
    contentText->setStyleSheet("font-size: 16px; color: #444; line-height: 1.6;");

    // 添加到滚动容器布局
    contentLayout->addWidget(bigImage);
    contentLayout->addWidget(authorWidget);
    contentLayout->addWidget(contentTitle);
    contentLayout->addWidget(contentText);
    contentLayout->addStretch(); // 底部弹簧

    scrollArea->setWidget(contentContainer);
    mainLayout->addWidget(scrollArea);
}
