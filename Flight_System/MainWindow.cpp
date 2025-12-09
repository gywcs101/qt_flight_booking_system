#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "adbanner.h" // 必须包含 adbanner.h
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ★★★ 关键修改 ★★★
    // 你的 UI 截图里，那个控件叫 "widget"，所以这里必须写 ui->widget
    
    // 假设 AdBanner 类里有 addImage 这个方法
    ui->bannerWidget->addImage(":/images/banner1.jpg");
    ui->bannerWidget->addImage("D:/test/ad2.jpg");
    ui->bannerWidget->addImage("D:/test/ad3.jpg");

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
