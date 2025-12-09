#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "adbanner.h"
#include <QDesktopServices>
#include <QUrl>
#include "UserCenter.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. 初始化轮播图 (bannerWidget)
    // 假设你有这些图片，如果没有请换成存在的图片路径
    ui->bannerWidget->addImage(":/adPicture1.png");
    ui->bannerWidget->addImage(":/adPicture2.png");
    // ui->bannerWidget->addImage(":/adPicture3.png"); // 有更多图片可以继续加

    // 启动轮播
    ui->bannerWidget->start(3000);

    ui->destinationWidget->setStyleSheet("border-image: url(:/destinationPic1.png); border-radius: 8px;");

    // 2. 设置第二个控件
    ui->destinationWidget2->setStyleSheet("border-image: url(:/destinationPic2.png); border-radius: 8px;");

    // 3. 设置第三个控件
    ui->destinationWidget3->setStyleSheet("border-image: url(:/destinationPic3.png); border-radius: 8px;");
}



MainWindow::~MainWindow()
{
    delete ui;
}

// 【新增】实现左侧菜单与右侧页面的联动
void MainWindow::on_menuList_currentRowChanged(int currentRow)
{
    // 根据行号切换到对应的页面
    // 你的命名非常规范，这里一一对应即可
    switch (currentRow) {
    case 0: ui->stackedWidget->setCurrentWidget(ui->page0_First);   break; // 首页
    case 1: ui->stackedWidget->setCurrentWidget(ui->page1_Flight);  break; // 全部航班
    case 2: ui->stackedWidget->setCurrentWidget(ui->page2_Special); break; // 特价机票
    case 3: ui->stackedWidget->setCurrentWidget(ui->page3_Find);    break; // 发现
    case 4: ui->stackedWidget->setCurrentWidget(ui->page4_Love);    break; // 我的收藏
    case 5: ui->stackedWidget->setCurrentWidget(ui->page5_Order);   break; // 我的订单
    case 6: ui->stackedWidget->setCurrentWidget(ui->page6_User);    break; // 用户中心
    case 7: ui->stackedWidget->setCurrentWidget(ui->page7_About);   break; // 关于我们
    default: break;
    }
}
