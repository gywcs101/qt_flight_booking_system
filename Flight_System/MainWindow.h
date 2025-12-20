#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

// ui 文件已经包含了所有页面类的头文件，这里不再需要向前声明

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    // [新增] 注销信号
    void logout();

private slots:
    void on_menuList_currentRowChanged(int currentRow);

    // [新增] 处理来自 UserCenter 的注销请求
    void handleUserLogout();

private:
    Ui::MainWindow *ui;
    // 不再需要 m_flightsPage, m_favPage, m_discoveryPage 等成员变量了
};

#endif // MAINWINDOW_H
