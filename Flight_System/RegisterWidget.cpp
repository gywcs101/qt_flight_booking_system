#include "RegisterWidget.h"
#include "ui_RegisterWidget.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// 注意：不要再包含 ConnectionManager.h

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
    // 获取输入内容，建议对用户名去空格
    QString user = ui->lineEdit_username->text().trimmed();
    QString pass = ui->lineEdit_password->text();
    QString confirm = ui->lineEdit_confirmPassword->text();

    // 1. 基础非空检查
    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写完整信息");
        return;
    }

    // 2. 确认密码一致性检查
    if (pass != confirm) {
        QMessageBox::warning(this, "错误", "两次输入的密码不一致");
        return;
    }

    // 3. 执行数据库注册 (替代 ConnectionManager)
    QSqlQuery query;
    // 假设数据库表名是 users，字段是 username 和 password
    // 如果你的表结构不一样，请修改下面的 SQL 语句
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(user);
    query.addBindValue(pass);

    if (query.exec()) {
        // 注册成功
        QMessageBox::information(this, "成功", "注册成功，请登录");
        emit goBackToLogin(); // 发送信号返回登录页
    } else {
        // 注册失败 (通常是用户名重复，或者数据库断开)
        qDebug() << "注册错误:" << query.lastError().text();
        QMessageBox::warning(this, "注册失败",
                             "无法完成注册，可能是用户名已存在。\n错误详情: " + query.lastError().text());
    }
}

void RegisterWidget::on_btn_cancel_clicked()
{
    emit goBackToLogin(); // 点击取消也返回登录页
}
