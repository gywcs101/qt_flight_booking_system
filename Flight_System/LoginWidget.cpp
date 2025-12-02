#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include "ConnectionManager.h"
#include <QMessageBox>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    // 这里的类名和 objectName 必须匹配，截图确认是 LoginWidget，所以没问题
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_btn_login_clicked()
{
    // 获取 UI 里的输入框内容
    QString user = ui->lineEdit_username->text();
    QString pass = ui->lineEdit_password->text();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名或密码不能为空");
        return;
    }

    User currentUser;
    // 调用数据库登录
    if (ConnectionManager::instance().login(user, pass, currentUser)) {
        emit loginSuccess(); // 发送信号给 main.cpp
    } else {
        QMessageBox::warning(this, "错误", "用户名或密码错误");
    }
}

void LoginWidget::on_pushButton_2_clicked()
{
    // 这是“取消”按钮，点击后关闭窗口
    this->close();
}

void LoginWidget::on_btn_register_jump_clicked()
{
    // 发送跳转信号
    emit goToRegister();
}
