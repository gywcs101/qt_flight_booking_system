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
    void goToRegister();
    void loginSuccess();

    // [核心修改] 新增一个专门给管理员登录成功时使用的信号
    void loginSuccessAsAdmin();

private slots:
    void on_btn_login_clicked();
    void on_pushButton_2_clicked();
    void on_btn_register_clicked();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
