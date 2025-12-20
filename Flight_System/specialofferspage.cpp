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

    // 清理旧的 items
    // m_scene->clear(); // 注意：这会把地图背景也清掉，如果背景是 addItem 加进去的要小心。
    // 建议只清理按钮，或者重新加载背景。假设这里只加载数据：

    QSqlQuery query("SELECT * FROM special_offers");
    while (query.next()) {
        QString city = query.value("city_name").toString();
        int price = query.value("price").toInt();
        int x = query.value("pos_x").toInt();
        int y = query.value("pos_y").toInt();
        // 确保数据库里有 img_url 字段，且路径正确（例如 ":/images/bj.jpg" 或 "D:/pic/1.jpg"）
        QString imgUrl = query.value("img_url").toString();

        // --- 创建按钮 ---
        MapMarkerBtn *tagBtn = new MapMarkerBtn(city, price, imgUrl);

        // --- 添加到场景 ---
        // addWidget 会返回一个代理控件 (QGraphicsProxyWidget)
        QGraphicsProxyWidget *proxy = m_scene->addWidget(tagBtn);

        // 设置代理控件在场景中的位置 (修正坐标：为了让尖端对准点，向左偏移一半宽度，向上偏移整个高度)
        // 假设按钮宽70，高55(含尖角)，尖角在底部中心
        proxy->setPos(x - 35, y - 55);
        proxy->setZValue(1); // 确保按钮在地图图片之上

        // ================== 【修改点 3：核心坐标计算逻辑】 ==================
        // 连接信号：使用 Lambda 表达式捕获 proxy 指针和 this 指针
        connect(tagBtn, &MapMarkerBtn::hoverEntered, this, [=](QString c, int p, QString url, QPoint){

            // 1. 设置卡片内容
            m_hoverCard->setContent(c, p, url);

            // 2. 计算位置：从 场景坐标 -> 视图坐标 -> 父窗口坐标
            // proxy->scenePos() 获取按钮在地图（场景）上的绝对位置
            QPointF scenePos = proxy->scenePos();

            // m_view->mapFromScene() 将场景坐标转换为 InteractiveMap 组件内的像素坐标
            QPoint viewPos = m_view->mapFromScene(scenePos);

            // m_view->mapTo() 将 View 内的坐标转换为 SpecialOffersPage (this) 的坐标
            // 因为 m_hoverCard 是 SpecialOffersPage 的子控件
            QPoint pagePos = m_view->mapTo(this, viewPos);

            // 3. 调整卡片位置，使其位于按钮正上方
            int cardW = m_hoverCard->width();
            int cardH = m_hoverCard->height();
            int btnW = tagBtn->width();

            // 计算最终 X：让卡片中心对准按钮中心
            int finalX = pagePos.x() + (btnW - cardW) / 2;

            // 计算最终 Y：卡片放在按钮上方，稍微留点空隙 (比如 5px)
            int finalY = pagePos.y() - cardH - 5;

            m_hoverCard->move(finalX, finalY);
            m_hoverCard->raise(); // 确保卡片在最上层
            m_hoverCard->show();
        });

        connect(tagBtn, &MapMarkerBtn::hoverLeft, this, [=](){
            m_hoverCard->hide();
        });
        // ==============================================================
    }
}
