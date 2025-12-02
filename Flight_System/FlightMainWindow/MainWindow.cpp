#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "homeform.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->menuList->setFrameShape(QFrame::NoFrame);

    // =======================================================
    // 2. 核心连接：列表点击 -> 切换页面
    // =======================================================
    // 原理：menuList的行号(0-7) 刚好对应 mainStack的页号(0-7)
    connect(ui->menuList, &QListWidget::currentRowChanged,
            ui->mainStack, &QStackedWidget::setCurrentIndex);

    // =======================================================
    // 3. 初始化状态：一打开就显示首页
    // =======================================================
    // 这行代码非常关键！它做了两件事：
    // 1. 将左侧菜单的第0项（首页）设置为“选中/按下”状态。
    // 2. 触发上面的 currentRowChanged 信号，自动让右侧切换到第0页。
    ui->menuList->setCurrentRow(0);

    while(ui->mainStack->count() > 0) {
        QWidget* widget = ui->mainStack->widget(0);
        ui->mainStack->removeWidget(widget);
        widget->deleteLater();
    }

    // 2. 实例化你的独立页面并按顺序加入
    ui->mainStack->addWidget(new homeForm(this));       // 索引 0 -> 首页
  // 索引 1 -> 全部航班
    // ... 依次添加剩下的 6 个页面 ...

    // 3. 连接信号
    connect(ui->menuList, &QListWidget::currentRowChanged,
            ui->mainStack, &QStackedWidget::setCurrentIndex);

    // 4. 设置默认选中
    ui->menuList->setCurrentRow(0);
}

Widget::~Widget()
{
    delete ui;
}
