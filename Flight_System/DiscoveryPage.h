#ifndef DISCOVERYPAGE_H
#define DISCOVERYPAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QSqlDatabase>
#include "PostData.h"

// 引用你的卡片类头文件，假设你的文件名为 PostCard.h
#include "PostCard.h"

class DiscoveryPage : public QWidget {
    Q_OBJECT

public:
    explicit DiscoveryPage(QWidget *parent = nullptr);
    ~DiscoveryPage();

private slots:
    // 接收卡片点击的槽函数
    void onCardClicked(const PostData &data);

private:
    void initUi();          // 初始化界面
    void connectDatabase(); // 连接数据库
    void loadData();        // 加载数据

    QSqlDatabase db;
    QWidget *gridWidget;    // 网格容器
    QGridLayout *gridLayout;// 网格布局
};

#endif // DISCOVERYPAGE_H
