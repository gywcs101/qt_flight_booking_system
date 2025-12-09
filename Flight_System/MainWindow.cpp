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
    ui->widget->addImage(":/images/banner1.jpg");
    ui->widget->addImage("D:/test/ad2.jpg");
    ui->widget->addImage("D:/test/ad3.jpg");

    // 启动轮播
    ui->widget->start(3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}