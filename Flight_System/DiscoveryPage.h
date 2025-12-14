#ifndef DISCOVERYPAGE_H
#define DISCOVERYPAGE_H

#include <QWidget>
#include <QGridLayout>
#include "PostData.h"

namespace Ui {
class DiscoveryPage;
}

class DiscoveryPage : public QWidget
{
    Q_OBJECT

public:
    explicit DiscoveryPage(QWidget *parent = nullptr);
    ~DiscoveryPage();

    // [新增] 添加一个公共的刷新函数，供 MainWindow 调用
    void refreshPosts();

private slots:
    void onCardClicked(const PostData &data);

private:
    void initUi();
    void loadData(); // 函数名改为 loadData

    Ui::DiscoveryPage *ui;

    // 不再需要 QSqlDatabase db; 成员变量
    QGridLayout *gridLayout;
};

#endif // DISCOVERYPAGE_H
