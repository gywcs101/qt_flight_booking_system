#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
class AllFlightsPage;
class FavoritesPage;
class UserCenter;

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 【新增】左侧菜单点击事件
    void on_menuList_currentRowChanged(int currentRow);

private:
    Ui::MainWindow *ui;
    AllFlightsPage *m_flightsPage;
    FavoritesPage *m_favPage;
    UserCenter *m_userPage;
};

#endif // MAINWINDOW_H
