#include "AdminWidget.h"
#include "ui_AdminWidget.h"

AdminWidget::AdminWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminWidget)
{
    ui->setupUi(this);
    // 初始化管理员界面（示例：设置窗口大小）
    this->setFixedSize(800, 600);
}

AdminWidget::~AdminWidget()
{
    delete ui;
}
