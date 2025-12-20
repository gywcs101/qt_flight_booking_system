#include "homeform.h"
#include "ui_homeform.h"
#include "adbanner.h" // 确保包含这个，否则不认识 AdBanner

homeForm::homeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::homeForm)
{
    ui->setupUi(this);

    // ==========================================
    // 1. 设置轮播图
    // ==========================================
    ui->bannerWidget->addImage(":/adPicture1.png");
    ui->bannerWidget->addImage(":/adPicture2.png");
    // ui->bannerWidget->addImage(":/adPicture3.png");
    ui->bannerWidget->start(3000);

    // ==========================================
    // 2. 设置下方三张热门卡片
    // ==========================================
    // 使用 border-image 可以自适应大小
    ui->dest1->setStyleSheet("border-image: url(:/destinationPic1.png); border-radius: 8px;");
    ui->dest2->setStyleSheet("border-image: url(:/destinationPic2.png); border-radius: 8px;");
    ui->dest3->setStyleSheet("border-image: url(:/destinationPic3.png); border-radius: 8px;");
}

homeForm::~homeForm()
{
    delete ui;
}
