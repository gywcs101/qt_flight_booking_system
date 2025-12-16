#include "AboutPage.h"
#include "ui_AboutPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QScrollArea>
#include <QDebug>

AboutPage::AboutPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutPage)
{
    ui->setupUi(this);

    // =======================================================
    // 1. 全局背景
    // =======================================================
    this->setStyleSheet("QWidget { background-color: #F5F7FA; border: none; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // =======================================================
    // 2. 滚动区域 (支持滑动浏览)
    // =======================================================
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *scrollContent = new QWidget();
    scrollContent->setStyleSheet(".QWidget { background-color: #F5F7FA; }");

    QVBoxLayout *contentLayout = new QVBoxLayout(scrollContent);
    contentLayout->setSpacing(20);
    contentLayout->setContentsMargins(40, 40, 40, 40);
    contentLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    // =======================================================
    // 3. 填充内容
    // =======================================================

    // --- Logo ---
    QLabel *logo = new QLabel(scrollContent);
    QPixmap pix(":/logo.png");
    if (!pix.isNull()) {
        logo->setPixmap(pix.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logo->setText("✈️");
        logo->setStyleSheet("font-size: 70px; background: transparent;");
    }
    logo->setAlignment(Qt::AlignCenter);

    // --- 标题 ---
    QLabel *title = new QLabel("Haavk Fly 航班系统", scrollContent);
    title->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 28px; font-weight: bold; color: #333; margin-top: 10px;");
    title->setAlignment(Qt::AlignCenter);

    QLabel *version = new QLabel("Version 1.0.0", scrollContent);
    version->setStyleSheet("color: #999; font-size: 14px; margin-bottom: 10px;");
    version->setAlignment(Qt::AlignCenter);

    // --- 简介 ---
    QLabel *desc = new QLabel(scrollContent);
    desc->setWordWrap(true);
    desc->setStyleSheet("font-size: 15px; color: #555; line-height: 160%; background: #FFFFFF; border-radius: 10px; padding: 20px;");
    desc->setText(
        "<p><b>Haavk Fly</b> 是一款为您精心打造的现代化智能航班管理系统。</p>"
        "<p>核心功能亮点包括：</p>"
        "<ul style='margin-left: -15px;'>"
        "<li>🚀 <b>实时航班</b>：精准覆盖热门航线，毫秒级响应。</li>"
        "<li>🌍 <b>发现探索</b>：查看旅游攻略与特价机票。</li>"
        "<li>💳 <b>便捷预订</b>：支持改签、退票及余额支付。</li>"
        "<li>❤️ <b>贴心管家</b>：专属收藏夹与订单管理。</li>"
        "</ul>"
        );
    desc->setMaximumWidth(600);

    // --- 开发者团队 ---
    QLabel *teamTitle = new QLabel("—— 开发团队 ——", scrollContent);
    teamTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #999; margin-top: 30px; margin-bottom: 10px;");
    teamTitle->setAlignment(Qt::AlignCenter);

    QHBoxLayout *teamLayout = new QHBoxLayout();
    teamLayout->setSpacing(40);
    teamLayout->setAlignment(Qt::AlignCenter);

    // 【修改点】只填名字和图片，不填职责
    teamLayout->addWidget(createDevProfile("吴兆临", ":/dev1.jpg"));
    teamLayout->addWidget(createDevProfile("方浩维", ":/dev2.jpg"));
    teamLayout->addWidget(createDevProfile("邱天宇", ":/dev3.jpg"));

    // --- 页脚 ---
    QLabel *footer = new QLabel("© 2025 Haavk Team. All rights reserved.", scrollContent);
    footer->setStyleSheet("color: #CCC; font-size: 12px; margin-top: 40px;");
    footer->setAlignment(Qt::AlignCenter);

    // 组装布局
    contentLayout->addWidget(logo);
    contentLayout->addWidget(title);
    contentLayout->addWidget(version);
    contentLayout->addWidget(desc);
    contentLayout->addWidget(teamTitle);
    contentLayout->addLayout(teamLayout);
    contentLayout->addWidget(footer);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}

AboutPage::~AboutPage()
{
    delete ui;
}

// 辅助函数：生成方形头像 (无职责文字)
QWidget* AboutPage::createDevProfile(QString name, QString imagePath)
{
    QWidget *widget = new QWidget(this);
    widget->setStyleSheet("background: transparent;");

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10); // 图片和名字之间的距离

    // 1. 头像 Label
    QLabel *avatar = new QLabel(widget);
    int size = 80; // 头像大小
    avatar->setFixedSize(size, size);

    QPixmap src(imagePath);
    if (src.isNull()) src.load(":/DefaultAvatar.png");

    // 创建透明画布
    QPixmap targetPixmap(size, size);
    targetPixmap.fill(Qt::transparent);

    QPainter painter(&targetPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 【核心】绘制圆角方形 (10px 圆角)
    QPainterPath path;
    path.addRoundedRect(0, 0, size, size, 10, 10);
    painter.setClipPath(path);

    painter.drawPixmap(0, 0, src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    avatar->setPixmap(targetPixmap);
    avatar->setAlignment(Qt::AlignCenter);

    // 2. 名字 Label
    QLabel *lblName = new QLabel(name, widget);
    lblName->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
    lblName->setAlignment(Qt::AlignCenter);

    // 只加头像和名字
    layout->addWidget(avatar, 0, Qt::AlignCenter);
    layout->addWidget(lblName);

    return widget;
}
