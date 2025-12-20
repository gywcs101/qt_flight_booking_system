#include "MainWindow.h"
#include "ui_MainWindow.h"

// --- [核心修改] 添加所有必要的头文件 ---
#include "LoginWidget.h"      // 解决 'LoginWidget' 未声明的错误
#include "UserCenter.h"       // 确保 UserCenter 类是已知的
#include "AllFlightsPage.h"   // 确保 AllFlightsPage 类是已知的
#include "DiscoveryPage.h"    // 确保 DiscoveryPage 类是已知的
#include "FavoritesPage.h"    // 确保 FavoritesPage 类是已知的
#include "adbanner.h"         // 确保 AdBanner 类是已知的
#include "MyOrdersPage.h"
#include <QDebug>             // 用于调试输出

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- 首页内容初始化 ---
    ui->bannerWidget->addImage(":/adPicture1.png");
    ui->bannerWidget->addImage(":/adPicture2.png");
    ui->bannerWidget->start(3000);
    ui->destinationWidget->setStyleSheet("border-image: url(:/destinationPic1.png); border-radius: 8px;");
    ui->destinationWidget2->setStyleSheet("border-image: url(:/destinationPic2.png); border-radius: 8px;");
    ui->destinationWidget3->setStyleSheet("border-image: url(:/destinationPic3.png); border-radius: 8px;");


    // --- 各个子页面的初始化 ---
    // 因为您在UI文件中使用了“提升(Promote)”，所以这里不需要手动 new 页面，这是正确的。
    // ui->setupUi() 已经帮我们完成了所有页面的实例化。

    // 确保程序启动时，默认选中第一个菜单项和页面
    ui->menuList->setCurrentRow(0);
    ui->stackedWidget->setCurrentIndex(0);

    // 连接 UserCenter 页面的退出登录信号到 MainWindow 的处理槽函数
    connect(ui->page6_User, &UserCenter::logoutSignal, this, &MainWindow::handleLogout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 左侧菜单项切换时的槽函数
void MainWindow::on_menuList_currentRowChanged(int currentRow)
{
    // 1. 直接根据菜单的行号，切换右侧 stackedWidget 显示的页面，确保同步
    ui->stackedWidget->setCurrentIndex(currentRow);

    // 2. 根据切换到的不同页面，执行该页面特定的数据刷新操作
    switch (currentRow) {
    case 0:
        // 首页，一般不需要特殊刷新
        break;
    case 1: // 全部航班页
        // 调用 AllFlightsPage 自己的成员函数来刷新数据
        ui->page1_Flight->loadFlightsData();
        break;
    case 2: // 特价机票页
        // 如果 page2_Special 也有刷新函数，在这里调用
        // ui->page2_Special->refreshSpecialOffers();
        break;
    case 3: // 发现页
        // 调用 DiscoveryPage 自己的成员函数来刷新数据
        ui->page3_Find->refreshPosts();
        break;
    case 4: // 我的收藏页
        // 调用 FavoritesPage 自己的成员函数来刷新数据
        ui->page4_Love->loadFavoriteFlights();
        break;

    case 5: // 我的订单页
        // 如果有订单页面类，在这里调用刷新
         ui->page5_Order->loadOrders();
        break;
    case 6: // 用户中心页
        // 调用 UserCenter 自己的成员函数来加载和显示当前用户信息
        ui->page6_User->loadCurrentUserData();
        break;
    case 7: // 关于我们页
        // 通常是静态页面，不需要刷新
        break;
    default:
        // 处理未知的菜单项
        qDebug() << "Unknown menu index:" << currentRow;
        break;
    }
}

// 处理退出登录信号的槽函数
void MainWindow::handleLogout()
{
    // 1. 关闭当前的主窗口
    this->close();

    // 2. 创建一个新的登录窗口实例并显示它
    //    程序流程回到登录界面
    LoginWidget *loginWindow = new LoginWidget();
    loginWindow->show();
}
