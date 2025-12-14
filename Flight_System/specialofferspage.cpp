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
    if (event->angleDelta().y() > 0) {
        scale(1.1, 1.1); // 放大 10%
    } else {
        scale(1.0 / 1.1, 1.0 / 1.1); // 缩小
    }
    // 不要调用父类的 wheelEvent，否则会触发滚动条滚动
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
    QPixmap mapPix(":/china_map.jpg");

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

    QSqlQuery query("SELECT * FROM special_offers"); // 确保数据库里有这个表
    while (query.next()) {
        QString city = query.value("city_name").toString();
        int price = query.value("price").toInt();
        int x = query.value("pos_x").toInt(); // 数据库里需要存坐标
        int y = query.value("pos_y").toInt();

        // 创建价格标签按钮
        QPushButton *tagBtn = new QPushButton();
        // 设置两行文字：城市\n价格
        tagBtn->setText(QString("%1\n¥%2").arg(city).arg(price));
        tagBtn->setFixedSize(60, 45);
        tagBtn->setCursor(Qt::PointingHandCursor);

        // 样式表
        tagBtn->setStyleSheet(
            "QPushButton {"
            // 渐变蓝色背景
            "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #4facfe, stop:1 #00f2fe);"
            // 或者纯色携程蓝： background-color: #2b92e4;
            "   color: white;"
            "   border-radius: 6px;"
            "   padding: 4px;"
            "   font-family: 'Microsoft YaHei';"
            "   font-size: 12px;"
            "   line-height: 14px;" // 调整行高
            "   border: 1px solid white;" // 白色描边增加对比度
            "}"
            "QPushButton:hover {"
            "   background-color: #FF9900;" // 悬停变橙色，强调特价
            "   border-color: #FF9900;"
            "   margin-top: -2px;" // 悬停时微微上浮效果
            "}"
            );

        connect(tagBtn, &QPushButton::clicked, [city, price](){
            qDebug() << "Clicked city:" << city << "Price:" << price;
            // 这里可以弹窗显示详情
        });

        // 将 Widget 添加到 GraphicsScene 中
        QGraphicsProxyWidget *proxy = m_scene->addWidget(tagBtn);

        // 设置位置
        proxy->setPos(x, y);

        // 确保标签浮在地图上面
        proxy->setZValue(1);
    }
}
