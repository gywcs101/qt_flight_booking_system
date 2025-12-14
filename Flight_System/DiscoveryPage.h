#ifndef DISCOVERYPAGE_H
#define DISCOVERYPAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QSqlDatabase>
#include "PostData.h"

// 1. 引入 UI 命名空间
namespace Ui {
class DiscoveryPage;
}

class DiscoveryPage : public QWidget
{
    Q_OBJECT

public:
    explicit DiscoveryPage(QWidget *parent = nullptr);
    ~DiscoveryPage();

private slots:
    void onCardClicked(const PostData &data);

private:
    void initUi();          // 初始化布局
    void connectDatabase(); // 连接数据库
    void loadData();        // 加载数据

    // 2. ui 指针
    Ui::DiscoveryPage *ui;

    QSqlDatabase db;
    QGridLayout *gridLayout; // 我们只需要保留布局指针，容器用ui里的
};

#endif // DISCOVERYPAGE_H
