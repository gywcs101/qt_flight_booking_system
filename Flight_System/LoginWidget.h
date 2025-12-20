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

signals:
    // 跳转注册页信号
    void goToRegister();

    // 普通用户登录成功信号
    void loginSuccess();

    // 管理员登录成功信号
    void loginSuccessAsAdmin();

private slots:
    void on_btn_login_clicked();
    void on_btn_cancel_clicked(); // 退出程序
    void on_btn_register_clicked(); // 注册

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
