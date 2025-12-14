#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

// 前置声明
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
    void on_menuList_currentRowChanged(int currentRow);

private:
    Ui::MainWindow *ui;

    // 声明页面指针
    AllFlightsPage *m_flightsPage;
    FavoritesPage *m_favPage;
    UserCenter *m_userPage;
};

#endif // MAINWINDOW_H
