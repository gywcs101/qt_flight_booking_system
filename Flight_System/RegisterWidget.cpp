#include "RegisterWidget.h"
#include "ui_RegisterWidget.h"
#include "ConnectionManager.h"
#include <QMessageBox>

RegisterWidget::RegisterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

void RegisterWidget::on_btn_register_clicked()
{
    QString user = ui->lineEdit_username->text();
    QString pass = ui->lineEdit_password->text();
    QString confirm = ui->lineEdit_confirmPassword->text();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写完整信息");
        return;
    }

    if (pass != confirm) {
        QMessageBox::warning(this, "错误", "两次输入的密码不一致");
        return;
    }

    // 调用数据库注册
    if (ConnectionManager::instance().registerUser(user, pass)) {
        QMessageBox::information(this, "成功", "注册成功，请登录");
        emit goBackToLogin(); // 注册成功后返回登录页
    } else {
        QMessageBox::warning(this, "失败", "注册失败，用户名可能已存在");
    }
}

void RegisterWidget::on_btn_cancel_clicked()
{
    emit goBackToLogin(); // 点击取消也返回登录页
}
