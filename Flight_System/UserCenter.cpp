#include "UserCenter.h"
#include "ui_UserCenter.h" // 假设有UI文件

// 构造函数实现
UserCenter::UserCenter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserCenter)
{
    ui->setupUi(this);
}

// 析构函数实现
UserCenter::~UserCenter()
{
    delete ui;
}
