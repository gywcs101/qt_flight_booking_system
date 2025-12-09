#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "adbanner.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), // 注意：这里调用 QWidget 构造
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->bannerWidget->addImage(":/adPicture1.png"); // 假设你有资源文件
    ui->bannerWidget->addImage(":/adPicture2.png");      // 或者本地路径

    // 2. 启动轮播
    ui->bannerWidget->start(3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
