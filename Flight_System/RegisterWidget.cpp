#include "RegisterWidget.h"
#include "ui_RegisterWidget.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression> // 用于校验电话号码格式

RegisterWidget::RegisterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);

    // =========================================================
    // ★★★ 核心设置：开启背景图显示 ★★★
    this->setAttribute(Qt::WA_StyledBackground);
    // =========================================================

    // 设置密码框为密文模式
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confirmPassword->setEchoMode(QLineEdit::Password);

    // 可选：限制电话框只能输入数字，且最多11位
    ui->lineEdit_phone->setMaxLength(11);
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

void RegisterWidget::on_btn_register_clicked()
{
    // 1. 获取输入内容，去除首尾空格
    QString user = ui->lineEdit_username->text().trimmed();
    QString pass = ui->lineEdit_password->text();
    QString confirm = ui->lineEdit_confirmPassword->text();
    QString phone = ui->lineEdit_phone->text().trimmed(); // 【新增】获取电话

    // 2. 基础非空检查
    if (user.isEmpty() || pass.isEmpty() || phone.isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写完整信息（用户名、密码、电话）");
        return;
    }

    // 3. 电话号码格式校验 (使用 QRegularExpression)
    // 这里的正则 ^1\d{10}$ 表示：以1开头，后面跟10个数字
    static const QRegularExpression rx("^1\\d{10}$");
    QRegularExpressionMatch match = rx.match(phone);

    if (!match.hasMatch()) { // 如果不匹配
        QMessageBox::warning(this, "格式错误", "请输入有效的 11 位手机号码");
        return;
    }

    // 4. 确认密码一致性检查
    if (pass != confirm) {
        QMessageBox::warning(this, "错误", "两次输入的密码不一致");
        return;
    }

    // 5. 查重逻辑（防止用户名重复）
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT username FROM users WHERE username = ?");
    checkQuery.addBindValue(user);
    if(checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "注册失败", "该用户名已存在，请更换。");
        return;
    }

    // 6. 执行数据库注册
    QSqlQuery query;

    // 【新增】 修改 SQL 语句，加入 phone 字段
    // 假设你的 users 表里字段顺序不需要严格对应，但字段名必须写对
    query.prepare("INSERT INTO users (username, password, phone, avatar_path) VALUES (?, ?, ?, ?)");

    query.addBindValue(user);
    query.addBindValue(pass);
    query.addBindValue(phone); // 【新增】 绑定电话号码
    query.addBindValue(":/DefaultAvatar.png"); // 绑定默认头像

    if (query.exec()) {
        QMessageBox::information(this, "成功", "注册成功，请登录");

        // 清空所有输入框
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_confirmPassword->clear();
        ui->lineEdit_phone->clear(); // 【新增】 清空电话框

        emit goBackToLogin(); // 发送信号返回登录页
    } else {
        qDebug() << "注册错误:" << query.lastError().text();
        // 这里的提示有助于调试，如果是因为数据库里没有 phone 字段，这里会显示出来
        QMessageBox::warning(this, "注册失败", "数据库错误: " + query.lastError().text());
    }
}

void RegisterWidget::on_btn_cancel_clicked()
{
    // 清空所有输入框
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_confirmPassword->clear();
    ui->lineEdit_phone->clear(); // 【新增】

    emit goBackToLogin();
}
