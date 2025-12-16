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
    // [核心修正] 槽函数名必须与 objectName 'listMenu' 对应
    void on_listMenu_currentRowChanged(int currentRow);

    // --- 用户管理槽函数 ---
    void on_tableUser_cellClicked(int row, int column);
    void on_btn_add_clicked();
    void on_btn_del_clicked();
    void on_btn_modify_clicked();

    // --- 管理员管理槽函数 ---
    void on_tableManager_cellClicked(int row, int column);
    void on_btn_add_2_clicked();
    void on_btn_del_2_clicked();
    void on_btn_modify_2_clicked();

    // --- 航班管理槽函数 ---
    void on_tableFlight_cellClicked(int row, int column);
    void on_btn_add_3_clicked();
    void on_btn_del_3_clicked();
    void on_btn_modify_3_clicked();

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
