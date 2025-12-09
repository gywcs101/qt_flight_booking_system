#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "adbanner.h"
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), // 注意：这里调用 QWidget 构造
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->bannerWidget->addImage(":/images/banner1.jpg"); // 假设你有资源文件
    ui->bannerWidget->addImage("D:/test/ad2.jpg");      // 或者本地路径
    ui->bannerWidget->addImage("D:/test/ad3.jpg");

    // 2. 启动轮播
    ui->bannerWidget->start(3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
