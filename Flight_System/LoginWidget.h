#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

    // --- 必须补上这部分 ---
signals:
    void loginSuccess();  // 登录成功的信号
    void goToRegister();  // 跳转去注册的信号
    // --------------------

private slots:
    void on_btn_login_clicked();

    void on_pushButton_2_clicked();
    void on_btn_register_clicked();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
