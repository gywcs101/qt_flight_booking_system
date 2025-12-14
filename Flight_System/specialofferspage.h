#ifndef SPECIALOFFERSPAGE_H
#define SPECIALOFFERSPAGE_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSqlDatabase>
#include <QWheelEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget> // 如果用了 proxy widget
#include <QPushButton>

// 1. 定义支持滚轮缩放的地图视图类
// (通常建议单独放一个文件，但为了方便先放在这里)
class InteractiveMap : public QGraphicsView {
    Q_OBJECT
public:
    explicit InteractiveMap(QWidget *parent = nullptr);

protected:
    // 重写滚轮事件
    void wheelEvent(QWheelEvent *event) override;
    // 在 InteractiveMap 类的 protected 下添加：
    void mousePressEvent(QMouseEvent *event) override;
};

// 2. 特价机票页面类
class SpecialOffersPage : public QWidget
{
    Q_OBJECT
public:
    explicit SpecialOffersPage(QWidget *parent = nullptr);
    ~SpecialOffersPage(); // 析构函数

private:
    void initUi();          // 初始化UI
    void connectDatabase(); // 连接数据库
    void loadMap();         // 加载地图
    void loadFlights();     // 加载航线数据

private:
    // 成员变量声明
    QGraphicsScene *m_scene;   // 场景
    InteractiveMap *m_view;    // 视图 (自定义的类)
    QSqlDatabase db;           // 数据库连接对象
};

#endif // SPECIALOFFERSPAGE_H
