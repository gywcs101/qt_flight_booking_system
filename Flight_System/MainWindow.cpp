#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "adbanner.h"
#include "AllFlightsPage.h" // 必须包含
#include "FavoritesPage.h"  // 必须包含
#include "UserCenter.h"     // 必须包含
#include <QVBoxLayout>      // 必须包含

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. 初始化轮播图
    ui->bannerWidget->addImage(":/adPicture1.png");
    ui->bannerWidget->addImage(":/adPicture2.png");
    ui->bannerWidget->start(3000);

    // 2. 设置图片
    ui->destinationWidget->setStyleSheet("border-image: url(:/destinationPic1.png); border-radius: 8px;");
    ui->destinationWidget2->setStyleSheet("border-image: url(:/destinationPic2.png); border-radius: 8px;");
    ui->destinationWidget3->setStyleSheet("border-image: url(:/destinationPic3.png); border-radius: 8px;");

    // ========================================================
    // 3. 【修复】初始化全部航班页面 (你之前漏了这一段)
    // ========================================================
    m_flightsPage = new AllFlightsPage(this);
    QVBoxLayout *layout1 = new QVBoxLayout(ui->page1_Flight);
    layout1->setContentsMargins(0, 0, 0, 0);
    layout1->addWidget(m_flightsPage);

    // ========================================================
    // 4. 初始化我的收藏页面
    // ========================================================
    m_favPage = new FavoritesPage(this);
    QVBoxLayout *layout4 = new QVBoxLayout(ui->page4_Love);
    layout4->setContentsMargins(0, 0, 0, 0);
    layout4->addWidget(m_favPage);

    // 5. (可选) 初始化用户中心
    // m_userPage = new UserCenter(this);
    // QVBoxLayout *layout6 = new QVBoxLayout(ui->page6_User);
    // layout6->setContentsMargins(0,0,0,0);
    // layout6->addWidget(m_userPage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_menuList_currentRowChanged(int currentRow)
{
    switch (currentRow) {
    case 0: ui->stackedWidget->setCurrentWidget(ui->page0_First);   break;
    case 1: ui->stackedWidget->setCurrentWidget(ui->page1_Flight);  break;
    case 2: ui->stackedWidget->setCurrentWidget(ui->page2_Special); break;
    case 3: ui->stackedWidget->setCurrentWidget(ui->page3_Find);    break;
    case 4:
        ui->stackedWidget->setCurrentWidget(ui->page4_Love);
        // 切换时刷新收藏数据
        if(m_favPage) m_favPage->loadFavoriteFlights();
        break;
    case 5: ui->stackedWidget->setCurrentWidget(ui->page5_Order);   break;
    case 6: ui->stackedWidget->setCurrentWidget(ui->page6_User);    break;
    case 7: ui->stackedWidget->setCurrentWidget(ui->page7_About);   break;
    default: break;
    }
}
