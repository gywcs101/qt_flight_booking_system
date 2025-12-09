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


}

MainWindow::~MainWindow()
{
    delete ui;
}
