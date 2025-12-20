#include "MainWindow.h"
#include "ui_MainWindow.h"

// 包含必要的头文件
#include "LoginWidget.h"
#include "UserCenter.h"
#include "AllFlightsPage.h"
#include "DiscoveryPage.h"
#include "FavoritesPage.h"
#include "adbanner.h"
#include "MyOrdersPage.h"
#include "UserSession.h" // [新增] 需要引用 UserSession 来清除ID
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- 首页内容初始化 ---
    ui->bannerWidget->addImage(":/adPicture1.png");
    ui->bannerWidget->addImage(":/adPicture2.png");
    ui->bannerWidget->start(3000);
    if(ui->destinationWidget) ui->destinationWidget->setStyleSheet("border-image: url(:/destinationPic1.png); border-radius: 8px;");
    if(ui->destinationWidget2) ui->destinationWidget2->setStyleSheet("border-image: url(:/destinationPic2.png); border-radius: 8px;");
    if(ui->destinationWidget3) ui->destinationWidget3->setStyleSheet("border-image: url(:/destinationPic3.png); border-radius: 8px;");

    // --- 页面初始化 ---
    ui->menuList->setCurrentRow(0);
    ui->stackedWidget->setCurrentIndex(0);

    // [核心修改] 连接信号到 handleUserLogout (注意名字要和头文件一致)
    connect(ui->page6_User, &UserCenter::logoutSignal, this, &MainWindow::handleUserLogout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_menuList_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);

    switch (currentRow) {
    case 0: break;
    case 1: ui->page1_Flight->loadFlightsData(); break;
    case 2: break;
    case 3: ui->page3_Find->refreshPosts(); break;
    case 4: ui->page4_Love->loadFavoriteFlights(); break;
    case 5: ui->page5_Order->loadOrders(); break;
    case 6: ui->page6_User->loadCurrentUserData(); break;
    case 7: break;
    default: qDebug() << "Unknown menu index:" << currentRow; break;
    }
}

// [核心修改] 处理退出登录
void MainWindow::handleUserLogout()
{
    // 1. 清除全局用户会话 ID (非常重要，防止下次登录还没输密码就显示旧数据)
    UserSession::instance().setUserId(-1);

    // 2. 隐藏主窗口
    this->hide();

    // 3. [关键] 发送信号给 main.cpp，而不是在这里 new 一个新的 LoginWidget
    //    让 main.cpp 去把最开始那个 w_login 窗口显示出来
    emit logout();
}
