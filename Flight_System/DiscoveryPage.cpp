#include "DiscoveryPage.h"
#include "ui_Discoverypage.h"
#include "PostCard.h" // 之前封装的卡片类
#include <QSqlQuery>
#include <QGridLayout>

DiscoveryPage::DiscoveryPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiscoveryPage)
{
    ui->setupUi(this);

    // 初始化时加载数据
    loadDataFromDB();
}

DiscoveryPage::~DiscoveryPage()
{
    delete ui;
}

void DiscoveryPage::loadDataFromDB()
{

}

void DiscoveryPage::refreshPosts() {
    // 清空旧数据重新加载的逻辑
    loadDataFromDB();
}
