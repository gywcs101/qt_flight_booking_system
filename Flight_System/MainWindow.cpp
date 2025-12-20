#include "MainWindow.h"
#include "ui_MainWindow.h"

// 引入子页面头文件
#include "LoginWidget.h"
#include "UserCenter.h"
#include "AllFlightsPage.h"
#include "DiscoveryPage.h"
#include "FavoritesPage.h"
#include "MyOrdersPage.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // =========================================================
    // 【修改点】首页初始化逻辑已移除
    // =========================================================
    // 之前的 ui->bannerWidget->addImage(...) 和 destinationWidget 设置
    // 已经被移除了。因为你已经在 UI 中把 page0_First 提升为 homeForm，
    // 这些逻辑现在由 homeForm 内部自动处理。
    // =========================================================


    // --- 页面初始化 ---
    ui->menuList->setCurrentRow(0);
    ui->stackedWidget->setCurrentIndex(0);

    // 连接 UserCenter 页面的退出登录信号 (保留原逻辑)
    connect(ui->page6_User, &UserCenter::logoutSignal, this, &MainWindow::handleLogout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 左侧菜单项切换时的槽函数
void MainWindow::on_menuList_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);

    switch (currentRow) {
    case 0:
        // 首页 (homeForm 自动管理，此处无需操作)
        break;
    case 1: // 全部航班页
        ui->page1_Flight->loadFlightsData();
        break;
    case 2: // 特价机票页
        // ui->page2_Special->refreshSpecialOffers();
        break;
    case 3: // 发现页
        ui->page3_Find->refreshPosts();
        break;
    case 4: // 我的收藏页
        ui->page4_Love->loadFavoriteFlights();
        break;
    case 5: // 我的订单页
        ui->page5_Order->loadOrders();
        break;
    case 6: // 用户中心页
        ui->page6_User->loadCurrentUserData();
        break;
    case 7: // 关于我们页
        break;
    default:
        qDebug() << "Unknown menu index:" << currentRow;
        break;
    }
}

// 处理退出登录信号的槽函数 (保留你原本的逻辑)
void MainWindow::handleLogout()
{
    // 1. 关闭当前的主窗口
    this->close();

    // 2. 创建一个新的登录窗口实例并显示它
    LoginWidget *loginWindow = new LoginWidget();
    loginWindow->show();
}
