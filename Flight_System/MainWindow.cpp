#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "adbanner.h"
#include "AllFlightsPage.h"
#include "FavoritesPage.h"
#include <QDesktopServices>
#include <QUrl>
#include "UserCenter.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. 初始化轮播图 (bannerWidget)
    // 从资源文件中加载图片
    ui->bannerWidget->addImage(":/adPicture1.png");
    ui->bannerWidget->addImage(":/adPicture2.png");
    // 如果有更多图片可以继续添加

    // 启动轮播，每3秒切换一次
    ui->bannerWidget->start(3000);

    // 2. 为 destination 区域的控件设置背景图片
    ui->destinationWidget->setStyleSheet("border-image: url(:/destinationPic1.png); border-radius: 8px;");
    ui->destinationWidget2->setStyleSheet("border-image: url(:/destinationPic2.png); border-radius: 8px;");
    ui->destinationWidget3->setStyleSheet("border-image: url(:/destinationPic3.png); border-radius: 8px;");

    // 1. 必须先 new 出来！否则 m_favPage 是空的
    m_favPage = new FavoritesPage(this);

    // 2. 把它加到布局里 (否则显示不出来)
    QVBoxLayout *layout4 = new QVBoxLayout(ui->page4_Love);
    layout4->setContentsMargins(0, 0, 0, 0);
    layout4->addWidget(m_favPage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 实现左侧菜单与右侧页面的联动
void MainWindow::on_menuList_currentRowChanged(int currentRow)
{
    // 根据左侧菜单被选中的行号，切换右侧 stackedWidget 显示的页面
    switch (currentRow) {
    case 0: ui->stackedWidget->setCurrentWidget(ui->page0_First);   break; // 首页
    case 1: ui->stackedWidget->setCurrentWidget(ui->page1_Flight);  break; // 全部航班
    case 2: ui->stackedWidget->setCurrentWidget(ui->page2_Special); break; // 特价机票
    case 3: ui->stackedWidget->setCurrentWidget(ui->page3_Find);    break; // 发现
    case 4:
        ui->stackedWidget->setCurrentWidget(ui->page4_Love);
        // 【绝对关键】每次切过来都要刷新
        if(m_favPage) m_favPage->loadFavoriteFlights();
        break; // 我的收藏
    case 5: ui->stackedWidget->setCurrentWidget(ui->page5_Order);   break; // 我的订单
    case 6: ui->stackedWidget->setCurrentWidget(ui->page6_User);    break; // 用户中心
    case 7: ui->stackedWidget->setCurrentWidget(ui->page7_About);   break; // 关于我们
    default: break;
    }
}
