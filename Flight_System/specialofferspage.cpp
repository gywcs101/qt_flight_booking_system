#include "specialofferspage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar> // 用于设置滚动条策略
// --- InteractiveMap 类的实现 ---

InteractiveMap::InteractiveMap(QWidget *parent) : QGraphicsView(parent) {
    // 开启拖拽模式
    setDragMode(QGraphicsView::ScrollHandDrag);
    // 优化渲染质量
    setRenderHint(QPainter::Antialiasing);
    // 去掉滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void InteractiveMap::wheelEvent(QWheelEvent *event) {
    // 获取当前水平方向的缩放比例
    double currentScale = transform().m11();

    // 定义限制范围
    double minScale = 0.5; // 最小缩放到 50%
    double maxScale = 3.0; // 最大放大到 300%

    // 如果想根据图片大小动态决定最小缩放，
    // 需要在 InteractiveMap 里存一下 scene 的 rect，这里先用固定值演示，效果通常足够。

    if (event->angleDelta().y() > 0) {
        // --- 准备放大 ---
        if (currentScale > maxScale) return; // 超过最大值，禁止放大
        scale(1.1, 1.1);
    } else {
        // --- 准备缩小 ---
        if (currentScale < minScale) return; // 超过最小值，禁止缩小
        scale(1.0 / 1.1, 1.0 / 1.1);
    }
}
void InteractiveMap::mousePressEvent(QMouseEvent *event) {
    // 调用父类，保证拖拽功能正常
    QGraphicsView::mousePressEvent(event);

    // 获取点击点在“场景(图片)”中的坐标
    QPointF scenePos = mapToScene(event->pos());
    qDebug() << "当前点击坐标 X:" << (int)scenePos.x() << " Y:" << (int)scenePos.y();
}

// --- SpecialOffersPage 类的实现 ---

SpecialOffersPage::SpecialOffersPage(QWidget *parent) : QWidget(parent)
{
    // 构造函数中依次调用初始化
    initUi();
    connectDatabase();
    loadMap();
    loadFlights();
}

SpecialOffersPage::~SpecialOffersPage()
{
    if(db.isOpen()) db.close();
}

void SpecialOffersPage::initUi()
{
    // 1. 设置主窗口背景色（浅灰，突出中间的内容）
    this->setStyleSheet("background-color: #F0F2F5;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20); // 页面留白
    mainLayout->setSpacing(15);

    // --- 顶部栏 ---
    QHBoxLayout *topLayout = new QHBoxLayout();

    QLabel *titleLabel = new QLabel("✈ 特价机票 · 探索世界", this);
    // 使用微软雅黑，深色字体
    titleLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 22px; font-weight: bold; color: #333;");

    QPushButton *btnReset = new QPushButton("⟲ 重置视角", this);
    // 扁平化按钮样式
    btnReset->setFixedSize(100, 35);
    btnReset->setCursor(Qt::PointingHandCursor);
    btnReset->setStyleSheet(
        "QPushButton {"
        "    background-color: white;"
        "    border: 1px solid #CCC;"
        "    border-radius: 17px;" // 圆角
        "    color: #666;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #E6F7FF;" // 悬停变淡蓝
        "    color: #0086F6;"
        "    border-color: #0086F6;"
        "}"
        );

    // 连接重置按钮
    connect(btnReset, &QPushButton::clicked, [this](){
        if(m_view && m_scene) {
            m_view->resetTransform();
            m_view->centerOn(m_scene->sceneRect().center());
        }
    });

    topLayout->addWidget(titleLabel);
    topLayout->addStretch(); // 弹簧，把按钮顶到右边
    topLayout->addWidget(btnReset);

    mainLayout->addLayout(topLayout);

    // --- 地图视图区域 ---
    m_scene = new QGraphicsScene(this);
    m_view = new InteractiveMap(this);
    m_view->setScene(m_scene);

    // 视图样式：白色背景，圆角边框，轻微阴影效果可以通过 qss 模拟
    m_view->setStyleSheet(
        "QGraphicsView {"
        "    border: none;" // 去掉默认边框
        "    background-color: #A3CCFF;" // 海洋色背景（如果地图没覆盖满）
        "    border-radius: 8px;"
        "}"
        );

    // 添加阴影效果（可选，需要包含 <QGraphicsDropShadowEffect>）
    // QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    // shadow->setBlurRadius(15);
    // shadow->setOffset(0, 0);
    // m_view->setGraphicsEffect(shadow);

    mainLayout->addWidget(m_view);

    // --- 初始化悬浮卡片 ---
    m_hoverCard = new CityDetailCard(this); // 父对象设为 this (QWidget)
    m_hoverCard->hide(); // 默认隐藏
    // 设置它不接受鼠标事件（让鼠标能穿透它点击下面的东西，可选）
    m_hoverCard->setAttribute(Qt::WA_TransparentForMouseEvents);
    // 确保卡片浮在最上层
    m_hoverCard->raise();
}
void SpecialOffersPage::connectDatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("flight_sys");
        db.setUserName("root");
        db.setPassword("123456");
    }

    if (!db.open()) {
        qDebug() << "Map DB Error:" << db.lastError().text();
    }
}

void SpecialOffersPage::loadMap()
{
    // 请确保路径正确，建议将图片放到资源文件(.qrc)中，路径用 ":/images/..."
    QPixmap mapPix(":/china_map2.png");

    if (mapPix.isNull()) {
        m_scene->addText("地图图片加载失败，请检查路径 D:/images/china_map.jpg");
        return;
    }

    // 将图片添加到场景
    m_scene->addPixmap(mapPix);
    // 设置场景大小等于图片大小
    m_scene->setSceneRect(mapPix.rect());
}

void SpecialOffersPage::loadFlights()
{
    if (!db.isOpen()) return;

    // 清理旧的 items (如果需要刷新功能的话)
    // m_scene->clear(); // 注意：这会把地图背景也清掉，这里假设只加载一次

    QSqlQuery query("SELECT * FROM special_offers");
    while (query.next()) {
        QString city = query.value("city_name").toString();
        int price = query.value("price").toInt();
        int x = query.value("pos_x").toInt();
        int y = query.value("pos_y").toInt();

        // 假设数据库里有一列叫 img_url，如果没有，你可以先用假数据
        // QString imgUrl = query.value("img_url").toString();
        QString imgUrl = QString(":/images/%1.jpg").arg(city); // 示例：从资源文件加载

        // --- 使用自定义的 MapMarkerBtn (要求2) ---
        MapMarkerBtn *tagBtn = new MapMarkerBtn(city, price, imgUrl);

        // --- 连接悬浮信号 (要求3) ---
        // 在 SpecialOffersPage::loadFlights() 的 connect 部分修改：
        connect(tagBtn, &MapMarkerBtn::hoverEntered, this, [this, tagBtn](QString c, int p, QString url, QPoint globalPos){
            m_hoverCard->setContent(c, p, url);
            // 原代码用了全局坐标，现在改为：基于按钮在地图中的局部坐标计算
            QPoint btnLocalPos = tagBtn->pos(); // 按钮在地图场景内的坐标
            // 让卡片与按钮中心对齐（核心修正）
            int cardX = btnLocalPos.x() - (m_hoverCard->width() / 2) + (tagBtn->width() / 2);
            int cardY = btnLocalPos.y() - (m_hoverCard->height() / 2) + (tagBtn->height() / 2);
            m_hoverCard->move(cardX, cardY);
            m_hoverCard->show();
        });

        connect(tagBtn, &MapMarkerBtn::hoverLeft, this, [this](){
            m_hoverCard->hide();
        });

        // 点击跳转逻辑保持不变
        connect(tagBtn, &QPushButton::clicked, [city, price](){
            qDebug() << "查看详情:" << city;
        });

        // 添加到场景
        QGraphicsProxyWidget *proxy = m_scene->addWidget(tagBtn);
        // 修正坐标：因为按钮坐标是左上角，为了让三角形尖端对准 (x,y)，需要向左偏移宽的一半，向上偏移整个高度
        proxy->setPos(x - 35, y - 55);
        proxy->setZValue(1);
    }
}
