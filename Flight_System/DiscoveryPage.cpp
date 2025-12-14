#include "DiscoveryPage.h"
#include "DetailDialog.h"
#include "PostCard.h"      // 必须有
#include "DetailDialog.h"  // 必须有
#include <QScrollArea>
#include <QLabel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QScrollBar>

DiscoveryPage::DiscoveryPage(QWidget *parent) : QWidget(parent) {
    initUi();
    connectDatabase();
    loadData();
}

DiscoveryPage::~DiscoveryPage() {
    if(db.isOpen()) {
        db.close();
    }
}

void DiscoveryPage::initUi() {
    // 1. 设置主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 2. 顶部标题栏
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(60);
    topBar->setStyleSheet("background-color: white; border-bottom: 1px solid #eee;");
    QHBoxLayout *topLayout = new QHBoxLayout(topBar);

    QLabel *titleLabel = new QLabel("发现", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #333; margin-left: 10px;");
    topLayout->addWidget(titleLabel);
    topLayout->addStretch();

    mainLayout->addWidget(topBar);

    // 3. 滚动区域 (核心部分)
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // 关键：让内部控件自适应宽度
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: #f7f7f7; border: none; }");

    // 优化滚动条样式 (可选)
    scrollArea->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical { width: 8px; background: transparent; }"
        "QScrollBar::handle:vertical { background: #ccc; border-radius: 4px; }"
        );

    // 4. 网格容器
    gridWidget = new QWidget();
    gridWidget->setStyleSheet("background-color: #f7f7f7;"); // 背景色

    gridLayout = new QGridLayout(gridWidget);
    gridLayout->setContentsMargins(20, 20, 20, 20); // 边距
    gridLayout->setSpacing(15); // 卡片之间的间距

    // 设置列比例，防止卡片被拉伸得很难看 (假设3列)
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 1);

    scrollArea->setWidget(gridWidget);
    mainLayout->addWidget(scrollArea);
}

void DiscoveryPage::connectDatabase() {
    // 检查是否已有连接，防止重复添加
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");       // 数据库地址
        db.setDatabaseName("flight_sys");  // 你的数据库名
        db.setUserName("root");            // 你的用户名
        db.setPassword("123456");          // 你的密码
        db.setPort(3306);
    }

    if (!db.open()) {
        qDebug() << "Error: Failed to connect database." << db.lastError();
    } else {
        qDebug() << "Database connected successfully.";
    }
}

void DiscoveryPage::loadData() {
    if (!db.isOpen()) return;

    // 清空现有布局（如果是刷新功能需要）
    QLayoutItem *child;
    while ((child = gridLayout->takeAt(0)) != 0) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    // 假设你的表名是 discovery_posts
    QSqlQuery query("SELECT * FROM discovery_posts ORDER BY id DESC");

    int row = 0;
    int col = 0;
    int maxColumns = 3; // 想要每行显示几个卡片，这里设为3

    while (query.next()) {
        PostData data;
        data.id = query.value("id").toInt();
        data.title = query.value("title").toString();
        data.content = query.value("content").toString();
        data.authorName = query.value("author_name").toString();
        data.imagePath = query.value("image_path").toString();
        data.avatarPath = query.value("avatar_path").toString();

        // 实例化你写好的 PostCard
        // 注意：这里假设你的 PostCard 构造函数接受 PostData
        // 如果你的 PostCard 构造函数不同，请在这里修改传参方式
        PostCard *card = new PostCard(data);

        // 连接点击信号
        // 假设你的 PostCard 有一个信号 cardClicked(PostData)
        connect(card, &PostCard::cardClicked, this, &DiscoveryPage::onCardClicked);

        // 添加到网格
        gridLayout->addWidget(card, row, col);

        // 计算行列索引
        col++;
        if (col >= maxColumns) {
            col = 0;
            row++;
        }
    }

    // 这是一个占位符，用于把卡片顶到最上面（如果只有一行不满时）
    if(row == 0 && col == 0) {
        // 没有数据
        QLabel *emptyLabel = new QLabel("暂无内容", gridWidget);
        emptyLabel->setAlignment(Qt::AlignCenter);
        gridLayout->addWidget(emptyLabel, 0, 0, 1, maxColumns);
    } else {
        gridLayout->setRowStretch(row + 1, 1);
    }
}

void DiscoveryPage::onCardClicked(const PostData &data) {
    // 弹出模态对话框
    DetailDialog *dialog = new DetailDialog(data, this);
    dialog->exec(); // 阻塞式显示
    delete dialog;  // 关闭后删除释放内存
}
