#include "bannerwidget.h"
#include <QVBoxLayout>

BannerWidget::BannerWidget(QWidget *parent)
    : QWidget(parent), m_currentIndex(0)
{
    initUI();

    // 设置定时器，每 3000 毫秒（3秒）切换一次
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &BannerWidget::onTimeout);
    m_timer->start(3000);
}

void BannerWidget::initUI()
{
    // 1. 布局设置
    // 使用 StackLayout 或者 Overlay 的思路，这里为了简单，用 VBox
    // 实际商业级开发建议用 paintEvent 或者 QStackedLayout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 去掉边距
    mainLayout->setSpacing(0);

    // 2. 图片显示控件
    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setScaledContents(true); // 图片自适应大小
    // 设置一个最小高度，防止太扁
    m_imageLabel->setMinimumHeight(200);
    m_imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 3. 底部小圆点容器
    m_dotContainer = new QWidget(this);
    m_dotContainer->setFixedHeight(40); // 底部高度
    // 给底部容器一个半透明背景，让小白点看得清
    m_dotContainer->setStyleSheet("background-color: rgba(0, 0, 0, 50);");

    QHBoxLayout *dotLayout = new QHBoxLayout(m_dotContainer);
    dotLayout->setContentsMargins(0, 0, 0, 0);
    dotLayout->setAlignment(Qt::AlignCenter); // 小圆点居中

    // 4. 添加到主布局
    mainLayout->addWidget(m_imageLabel);
    // 注意：这里为了简单演示，把圆点放在了图片下方。
    // 如果想让圆点浮在图片上，需要使用 QStackedLayout 或设置父子关系并移动坐标。
    mainLayout->addWidget(m_dotContainer);
}

void BannerWidget::setImagePaths(const QStringList &paths)
{
    m_pathList = paths;
    m_currentIndex = 0;

    // 清理旧的圆点
    qDeleteAll(m_dots);
    m_dots.clear();

    // 获取圆点布局
    QLayout *layout = m_dotContainer->layout();

    // 根据图片数量创建小圆点
    for (int i = 0; i < paths.count(); ++i) {
        QPushButton *btn = new QPushButton(this);
        btn->setFixedSize(12, 12); // 圆点大小
        btn->setCursor(Qt::PointingHandCursor);

        // 设置圆点样式（圆形，默认灰色）
        btn->setStyleSheet(
            "QPushButton { "
            "    border-radius: 6px; "
            "    background-color: rgba(255, 255, 255, 150); "
            "    border: none; "
            "}"
            );

        // 绑定点击事件，点击圆点切换到对应图片
        // 这里的 setProperty 是为了知道这个按钮对应第几张图
        btn->setProperty("index", i);
        connect(btn, &QPushButton::clicked, this, &BannerWidget::onDotClicked);

        layout->addWidget(btn);
        m_dots.append(btn);
    }

    updateImage();
}

void BannerWidget::onTimeout()
{
    if (m_pathList.isEmpty()) return;

    m_currentIndex++;
    if (m_currentIndex >= m_pathList.count()) {
        m_currentIndex = 0;
    }
    updateImage();
}

void BannerWidget::onDotClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        // 获取点击的圆点对应的索引
        int index = btn->property("index").toInt();
        m_currentIndex = index;
        updateImage();

        // 点击后重置定时器，避免刚刚点完马上又自动切
        m_timer->start(3000);
    }
}

void BannerWidget::updateImage()
{
    if (m_pathList.isEmpty()) return;

    // 1. 切换图片
    QPixmap pix(m_pathList[m_currentIndex]);
    m_imageLabel->setPixmap(pix);

    // 2. 切换圆点样式
    for (int i = 0; i < m_dots.count(); ++i) {
        if (i == m_currentIndex) {
            // 选中状态：纯白，稍微大一点（视觉效果）
            m_dots[i]->setStyleSheet(
                "border-radius: 6px; background-color: #FFFFFF;"
                );
        } else {
            // 未选中状态：半透明灰
            m_dots[i]->setStyleSheet(
                "border-radius: 6px; background-color: rgba(255, 255, 255, 100);"
                );
        }
    }
}
