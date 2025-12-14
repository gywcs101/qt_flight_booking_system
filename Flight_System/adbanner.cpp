#include "adbanner.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPixmap>
#include <QShowEvent>
#include <QHideEvent>

AdBanner::AdBanner(QWidget *parent) : QWidget(parent), m_currentIndex(0)
{
    // 1. 设置主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 2. 图片显示区域
    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setScaledContents(true); // 图片铺满且自适应
    m_imageLabel->setMinimumHeight(150);   // 设置一个最小高度

    // 3. 底部小圆点区域
    m_indicatorContainer = new QWidget(this);
    m_indicatorContainer->setFixedHeight(30);
    m_indicatorContainer->setStyleSheet("background-color: rgba(0,0,0,50);");

    m_indicatorLayout = new QHBoxLayout(m_indicatorContainer);
    m_indicatorLayout->setContentsMargins(0,0,0,0);
    m_indicatorLayout->setAlignment(Qt::AlignCenter);

    m_btnGroup = new QButtonGroup(this);
    connect(m_btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(onDotClicked(int)));

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

    if (id == 0) {
        dot->setChecked(true);
        updateImage();
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
    if (m_imagePaths.isEmpty() || m_currentIndex >= m_imagePaths.count()) return;
    QPixmap pix(m_imagePaths[m_currentIndex]);
    m_imageLabel->setPixmap(pix);
    if(m_btnGroup->button(m_currentIndex)) {
        m_btnGroup->button(m_currentIndex)->setChecked(true);
    }
}

void AdBanner::onDotClicked(int index)
{
    if (index >= 0 && index < m_imagePaths.count()) {
        m_currentIndex = index;
        updateImage();
        if(m_timer->isActive()) {
            m_timer->start(); // 重置计时
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
    if(!m_imagePaths.isEmpty() && this->isVisible()) {
        m_timer->start();
    }
}

void AdBanner::hideEvent(QHideEvent *e) {
    m_timer->stop();
    QWidget::hideEvent(e);
}

void AdBanner::setImagePaths(const QStringList &paths)
{
    // 清空旧数据
    m_imagePaths.clear();
    QLayoutItem *child;
    while ((child = m_indicatorLayout->takeAt(0)) != nullptr) {
        if(child->widget()){
            delete child->widget();
        }
        delete child;
    }
    // 遍历新列表，一个个加进去
    for(const QString &path : paths) {
        addImage(path);
    }
}
