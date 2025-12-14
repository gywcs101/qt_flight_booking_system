#include "DiscoveryPage.h"
#include "ui_DiscoveryPage.h" // 1. 必须引入这个由.ui生成的头文件
#include "PostCard.h"
#include "DetailDialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DiscoveryPage::DiscoveryPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiscoveryPage) // 2. 初始化 ui 指针
{
    ui->setupUi(this); // 3. 构建界面 (加载你在设计器里拖的控件)

    initUi();
    connectDatabase();
    loadData();
}

DiscoveryPage::~DiscoveryPage()
{
    if(db.isOpen()) db.close();
    delete ui; // 记得删除
}

void DiscoveryPage::initUi()
{
    // 【核心修改】
    // 原来的代码是自己 new 滚动区域，现在直接利用 UI 文件里已经有的控件

    // ui->scrollArea 是外面的框
    // ui->scrollAreaWidgetContents 是里面那个白色的画布

    // 我们在这个画布上创建一个网格布局
    // 检查是否已经有布局（防止重复设置）
    if (!ui->scrollAreaWidgetContents->layout()) {
        gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    } else {
        gridLayout = qobject_cast<QGridLayout*>(ui->scrollAreaWidgetContents->layout());
    }

    // 设置布局参数，让卡片排列整齐
    gridLayout->setContentsMargins(20, 20, 20, 20); // 上下左右留白
    gridLayout->setSpacing(20); // 卡片之间的间距

    // 设置列拉伸比例 (3列模式)
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 1);

    // 这里的 ui->scrollArea 样式可以在这里通过代码加强一下，或者你在设计器里设好了也行
    ui->scrollArea->setFrameShape(QFrame::NoFrame); // 去掉边框更现代
}

void DiscoveryPage::connectDatabase()
{
    // 数据库连接逻辑不变
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("flight_sys"); // 确认你的库名
        db.setUserName("root");           // 确认你的用户名
        db.setPassword("123456");         // 确认你的密码
    }

    if (!db.open()) {
        qDebug() << "DB Connect Error:" << db.lastError().text();
    }
}

void DiscoveryPage::loadData()
{
    if(!db.isOpen()) return;

    // 清空现有布局（防止刷新时重叠）
    QLayoutItem *child;
    while ((child = gridLayout->takeAt(0)) != 0) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    QSqlQuery query("SELECT * FROM discovery_posts");

    int row = 0;
    int col = 0;
    int maxCols = 3; // 每行显示3个

    while (query.next()) {
        PostData data;
        // 确保这些字段名和你数据库里的一样
        data.id = query.value("id").toInt();
        data.title = query.value("title").toString();
        data.content = query.value("content").toString();
        data.authorName = query.value("author_name").toString();
        data.imagePath = query.value("image_path").toString();
        data.avatarPath = query.value("avatar_path").toString();

        // 创建卡片
        PostCard *card = new PostCard(data);

        // 连接信号槽
        connect(card, &PostCard::cardClicked, this, &DiscoveryPage::onCardClicked);

        // 【关键】把卡片添加到我们刚才在 initUi 里建好的 gridLayout 中
        gridLayout->addWidget(card, row, col);

        // 计算下一个位置
        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }

    // 添加弹簧，把卡片顶到顶部（防止只有一行时分散对齐）
    if (row > 0 || col > 0) {
        gridLayout->setRowStretch(row + 1, 1);
    }
}

void DiscoveryPage::onCardClicked(const PostData &data)
{
    // 弹窗逻辑不变
    DetailDialog *dialog = new DetailDialog(data, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}
