#include "homeform.h"
#include "ui_homeform.h"

#include <QVBoxLayout>   // ⭐ 必须引入
#include <QSizePolicy>

homeForm::homeForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::homeForm)
{
    ui->setupUi(this);

    // ================================
    // 【核心第③步】建立 layout 管理 adbanner
    // ================================

    // 1. 给 homeForm 创建一个垂直布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 2. 去掉边距，确保真正铺满
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 3. 把 adbanner 放进 layout
    mainLayout->addWidget(ui->adbanner);

    // 4. 明确告诉 layout：它吃掉所有多余空间
    mainLayout->setStretch(0, 1);

    // 5. 再保险一次：设置 sizePolicy
    ui->adbanner->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);

    // ================================
    // Banner 数据初始化
    // ================================
    QStringList images;
    images << ":/adPicture1.png"
           << ":/adPicture2.png";

    ui->adbanner->setImagePaths(images);
}

homeForm::~homeForm()
{
    delete ui;
}
