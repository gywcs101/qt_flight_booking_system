#include "adbanner.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPixmap>
#include <QShowEvent>
#include <QHideEvent>
#include <QResizeEvent>
#include <QSizePolicy> // 【新增】必须引入这个头文件

AdBanner::AdBanner(QWidget *parent) : QWidget(parent), m_currentIndex(0)

{
     this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // 1. 设置主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 2. 图片显示区域
    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setScaledContents(false); // 允许图片缩放填充

    // ==========================================================
    // 【核心修改】设置尺寸策略，让图片随窗口拉伸
    // Horizontal: Ignored (忽略图片原宽，随父窗口水平拉伸)
    // Vertical: Expanding (尽可能占据垂直空间)
    // ==========================================================
    m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

    m_imageLabel->setMinimumHeight(150);   // 设置一个最小高度防止完全消失

    // 3. 底部小圆点区域
    m_indicatorContainer = new QWidget(this);
    m_indicatorContainer->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Minimum
        );
    m_indicatorContainer->setStyleSheet("background-color: rgba(0,0,0,50);");

    m_indicatorLayout = new QHBoxLayout(m_indicatorContainer);
    m_indicatorLayout->setContentsMargins(0,0,0,0);
    m_indicatorLayout->setAlignment(Qt::AlignCenter);

    m_btnGroup = new QButtonGroup(this);
    // 注意：QButtonGroup::buttonClicked(int) 是 Qt5/6 通用的重载信号，建议用旧式宏或函数指针转换
   connect(m_btnGroup, &QButtonGroup::idClicked, this, &AdBanner::onDotClicked);

    // 4. 添加到主布局
    mainLayout->addWidget(m_imageLabel);
    mainLayout->addWidget(m_indicatorContainer);

    // 5. 定时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &AdBanner::showNextImage);
}

void AdBanner::addImage(const QString &imagePath)
{
    m_imagePaths.append(imagePath);
    int id = m_imagePaths.count() - 1;

    // 创建小圆点 (用QRadioButton模拟)
    QRadioButton *dot = new QRadioButton(this);
    dot->setStyleSheet(
        "QRadioButton::indicator { width: 10px; height: 10px; border-radius: 5px; background-color: white; border: 1px solid gray; }"
        "QRadioButton::indicator:checked { background-color: #0078d7; border: 1px solid #0078d7; }"
        );
    m_indicatorLayout->addWidget(dot);
    m_btnGroup->addButton(dot, id);

    // 如果是第一张图，默认选中并显示
    if (id == 0) {
        dot->setChecked(true);
        updateImage();
    }
}

void AdBanner::setImagePaths(const QStringList &paths)
{
    // 1. 清空旧数据
    m_imagePaths.clear();
    m_currentIndex = 0;

    // 2. 清空旧的小圆点控件
    QLayoutItem *child;
    while ((child = m_indicatorLayout->takeAt(0)) != nullptr) {
        if(child->widget()){
            delete child->widget();
        }
        delete child;
    }

    // 3. 重新添加
    for(const QString &path : paths) {
        addImage(path);
    }
}

void AdBanner::showNextImage()
{
    if (m_imagePaths.isEmpty()) return;
    m_currentIndex = (m_currentIndex + 1) % m_imagePaths.count();
    updateImage();
}

void AdBanner::updateImage()
{
    if (m_imagePaths.isEmpty()) return;
    if (m_currentIndex >= m_imagePaths.count()) m_currentIndex = 0;

    QPixmap pix(m_imagePaths[m_currentIndex]);
    if (pix.isNull()) return;

    // 🔥 核心修复：按当前 QLabel 尺寸缩放
    QPixmap scaled = pix.scaled(
        m_imageLabel->size(),
        Qt::IgnoreAspectRatio,          // 铺满（不管比例）
        Qt::SmoothTransformation
        );

    m_imageLabel->setPixmap(scaled);

    if (m_btnGroup->button(m_currentIndex)) {
        m_btnGroup->button(m_currentIndex)->setChecked(true);
    }
}

void AdBanner::onDotClicked(int index)
{
    if (index >= 0 && index < m_imagePaths.count()) {
        m_currentIndex = index;
        updateImage();
        // 如果用户手动点击了，重置定时器，避免马上又切图
        if(m_timer->isActive()) {
            m_timer->start();
        }
    }
}

void AdBanner::start(int interval) {
    if(!m_imagePaths.isEmpty()) {
        m_timer->start(interval);
    }
}

void AdBanner::stop() {
    m_timer->stop();
}

void AdBanner::showEvent(QShowEvent *e) {
    QWidget::showEvent(e);
    // 窗口显示时自动开始轮播
    if(!m_imagePaths.isEmpty() && !m_timer->isActive()) {
        m_timer->start();
    }
}

void AdBanner::hideEvent(QHideEvent *e) {
    // 窗口隐藏时自动停止轮播，节省CPU
    m_timer->stop();
    QWidget::hideEvent(e);
}

void AdBanner::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // 尺寸变化时，重新按当前 QLabel 大小设置图片
    updateImage();
}
