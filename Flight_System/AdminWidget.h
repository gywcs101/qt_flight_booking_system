#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>

namespace Ui {
class AdminWidget;
}

class AdminWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWidget(QWidget *parent = nullptr);
    ~AdminWidget();

private slots:
    void on_listMenu_currentRowChanged(int currentRow);

    // --- 用户管理 ---
    void on_tableUser_cellClicked(int row, int column);
    void on_btn_add_clicked();
    void on_btn_del_clicked();
    void on_btn_modify_clicked();
    void on_btn_search_clicked(); // [新增] 用户搜索

    // --- 管理员管理 ---
    void on_tableManager_cellClicked(int row, int column);
    void on_btn_add_2_clicked();
    void on_btn_del_2_clicked();
    void on_btn_modify_2_clicked();
    void on_btn_search_2_clicked(); // [新增] 管理员搜索

    // --- 航班管理 ---
    void on_tableFlight_cellClicked(int row, int column);
    void on_btn_add_3_clicked();
    void on_btn_del_3_clicked();
    void on_btn_modify_3_clicked();
    void on_btn_search_3_clicked(); // [新增] 航班搜索

private:
    void setupUiCustom();

    void loadUsers();
    void loadAdmins();
    void loadFlights();

    void clearUserInputs();
    void clearAdminInputs();
    void clearFlightInputs();

private:
    Ui::AdminWidget *ui;

    int m_selectedUserId;
    int m_selectedAdminId;
    QString m_selectedFlightId;
};

#endif // ADMINWIDGET_H
