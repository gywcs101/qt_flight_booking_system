#include "DiscoveryPage.h"
#include "ui_DiscoveryPage.h"
#include "PostCard.h"
#include "DetailDialog.h"
#include "ODBC.h"           // [修改] 引入您全局的 ODBC 类
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DiscoveryPage::DiscoveryPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiscoveryPage)
{
    ui->setupUi(this);
    initUi();
    // [修改] 不再自己连接数据库，依赖全局的 ODBC 连接
    loadData();
}

DiscoveryPage::~DiscoveryPage()
{
    delete ui;
}

void DiscoveryPage::initUi()
{
    // 检查是否已经有布局（防止重复设置）
    if (!ui->scrollAreaWidgetContents->layout()) {
        gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    } else {
        gridLayout = qobject_cast<QGridLayout*>(ui->scrollAreaWidgetContents->layout());
    }

    gridLayout->setContentsMargins(20, 20, 20, 20);
    gridLayout->setSpacing(20);

    // 设置3列等宽
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 1);

    ui->scrollArea->setFrameShape(QFrame::NoFrame);
}

// [删除] 不再需要独立的 connectDatabase() 函数
/*
void DiscoveryPage::connectDatabase() { ... }
*/

void DiscoveryPage::loadData()
{
    // 清空现有布局（防止刷新时重叠）
    QLayoutItem *child;
    while ((child = gridLayout->takeAt(0)) != nullptr) {
        if(child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    // [修改] 使用全局的 ODBC::query() 函数来执行查询
    // 请将 "discovery_posts" 替换为您的真实表名
    QSqlQuery query = ODBC::query("SELECT * FROM discovery_posts ORDER BY id DESC");

    if(!query.isActive()){
        qWarning() << "DiscoveryPage: Failed to execute query." << query.lastError();
        return;
    }

    int row = 0;
    int col = 0;
    const int maxCols = 3;

    while (query.next()) {
        PostData data;
        // 确保这些列名与您的数据库表完全一致
        data.id = query.value("id").toInt();
        data.title = query.value("title").toString();
        data.content = query.value("content").toString();
        data.authorName = query.value("author_name").toString(); // 假设有 author_name 列
        data.imagePath = query.value("image_path").toString();   // 假设有 image_path 列
        data.avatarPath = query.value("avatar_path").toString(); // 假设有 avatar_path 列

        PostCard *card = new PostCard(data);
        connect(card, &PostCard::cardClicked, this, &DiscoveryPage::onCardClicked);
        gridLayout->addWidget(card, row, col);

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }

    // 添加弹簧，把卡片顶到顶部
    gridLayout->setRowStretch(row + 1, 1);
}

// [新增] refreshPosts() 函数的实现
void DiscoveryPage::refreshPosts()
{
    // 刷新的核心就是重新加载数据
    loadData();
}


void DiscoveryPage::onCardClicked(const PostData &data)
{
    DetailDialog *dialog = new DetailDialog(data, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}
