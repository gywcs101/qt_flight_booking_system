#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), // 注意：这里调用 QWidget 构造
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化列表或其他逻辑
    // 例如默认选中第一项
    // ui->menuList->setCurrentRow(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
