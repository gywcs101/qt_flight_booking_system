#include "AboutPage.h"
#include "ui_AboutPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>

AboutPage::AboutPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutPage)
{
    ui->setupUi(this);

    // =======================================================
    // 1. 全局背景美化
    // =======================================================
    // 使用淡灰色背景，让页面看起来更干净、专业
    this->setStyleSheet("QWidget#AboutPage { background-color: #F5F7FA; }");

    // =======================================================
    // 2. 布局初始化
    // =======================================================
    // 为了防止 UI 文件里没拖布局导致控件重叠，这里代码手动接管布局
    if (!this->layout()) {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(20);
        mainLayout->setContentsMargins(40, 40, 40, 40); // 四周留白
        mainLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop); // 居中靠上
    }

    // 获取布局指针（如果你在UI里拖了verticalLayout，这里会自动获取到，否则用上面新建的）
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->layout());

    // =======================================================
    // 3. UI 元素构建
    // =======================================================

    // --- A. Logo 图标 ---
    QLabel *logo = new QLabel(this);
    // 尝试加载 logo 图片，如果没有图片则显示 Emoji
    QPixmap pix(":/logo.png");
    if (!pix.isNull()) {
        logo->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logo->setText("✈️"); // 备用图标
        logo->setStyleSheet("font-size: 80px; background: transparent;");
    }
    logo->setAlignment(Qt::AlignCenter);

    // --- B. 项目标题 ---
    QLabel *title = new QLabel("Haavk Fly 航班系统", this);
    title->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 32px; font-weight: bold; color: #333; margin-top: 10px;");
    title->setAlignment(Qt::AlignCenter);

    // --- C. 版本信息 ---
    QLabel *version = new QLabel("Version 1.0.0 (Release 2025)", this);
    version->setStyleSheet("color: #999; font-size: 14px; margin-bottom: 20px;");
    version->setAlignment(Qt::AlignCenter);

    // --- D. 核心介绍 (包含发现页与特价机票文案) ---
    QLabel *content = new QLabel(this);
    content->setWordWrap(true); // 允许自动换行
    content->setOpenExternalLinks(true); // 允许点击链接
    content->setStyleSheet("font-size: 16px; color: #555; line-height: 160%;"); // 增加行高，提升阅读体验

    // 使用 HTML 富文本排版
    content->setText(
        "<p style='text-align: center;'><b>Haavk Fly</b> 是一款为您精心打造的现代化智能航班管理系统。</p>"
        "<p>我们致力于连接您与世界，核心功能亮点包括：</p>"
        "<ul style='margin-left: 20px;'>"
        "<li>🚀 <b>实时航班查询</b>：毫秒级响应，精准覆盖国内热门航线，数据实时同步。</li>"
        "<li>🌍 <b>发现与探索</b>：沉浸式浏览<b>旅游攻略</b>，实时捕捉<b>全球特价机票</b>，让每一次出发都物超所值。</li>"
        "<li>💳 <b>无忧预订体验</b>：支持一站式购票、余额支付，提供便捷的<b>改签</b>与<b>自动退款</b>服务。</li>"
        "<li>❤️ <b>贴心行程管家</b>：专属收藏夹监控票价波动，个人中心定制专属行程，让旅行更从容。</li>"
        "</ul>"
        "<p style='margin-top:15px; color:#888; font-size:14px; text-align: center;'>"
        "技术栈：Qt 6.x (C++) + MySQL + ODBC + QSS UI Design"
        "</p>"
        );
    // 给文字内容设置一点内边距，不要太贴边
    content->setContentsMargins(20, 0, 20, 0);

    // --- E. 页脚版权 ---
    QLabel *footer = new QLabel("© 2025 Haavk Dev Team. All rights reserved.\nContact: support@haavk-fly.com", this);
    footer->setStyleSheet("color: #CCC; font-size: 12px; margin-top: 40px;");
    footer->setAlignment(Qt::AlignCenter);

    // =======================================================
    // 4. 添加到布局
    // =======================================================
    layout->addWidget(logo);
    layout->addWidget(title);
    layout->addWidget(version);
    layout->addWidget(content);
    layout->addStretch(); // 弹簧：把版权信息顶到底部
    layout->addWidget(footer);
}

AboutPage::~AboutPage()
{
    delete ui;
}
