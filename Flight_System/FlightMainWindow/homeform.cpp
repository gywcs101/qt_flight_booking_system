#include "homeform.h"
#include "ui_homeform.h"

homeForm::homeForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::homeForm)
{
    ui->setupUi(this);

    QStringList images;
    images << ":/adPicture1.png" << ":/adPicture2.png";

    // ui->widgetBanner 就是刚才你在 UI 里提升的那个控件
    // 因为提升了，它有了 BannerWidget 的方法
    ui->widgetBanner->setImagePaths(images);
}

homeForm::~homeForm()
{
    delete ui;
}
