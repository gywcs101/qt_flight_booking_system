#include "ResetPasswordWidget.h"
#include "ui_ResetPasswordWidget.h"
#include "ODBC.h"
#include "UserSession.h"

// --- [核心修改] 添加以下所有缺失的头文件 ---
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QLineEdit> // <-- 编译器不认识 QLineEdit，因为你没包含它

ResetPasswordWidget::ResetPasswordWidget(int userId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetPasswordWidget),
    m_userId(userId)
{
    ui->setupUi(this);
    this->setWindowTitle("修改密码");
    this->setAttribute(Qt::WA_StyledBackground, true);
    // --- 初始化界面 ---

    // 1. 设置密码输入框为密码模式 (显示 ●●●)
    // 根据您的 UI 截图中的 objectName
    ui->lineEdit_originalPassword->setEchoMode(QLineEdit::Password);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confirmPassword->setEchoMode(QLineEdit::Password);

    // 2. 显示当前用户名 (让用户确认没找错账号)
    // 我们需要从数据库查询当前用户名
    QSqlQuery query = ODBC::query(QString("SELECT username FROM users WHERE id = %1").arg(m_userId));
    if (query.next()) {
        ui->label->setText(query.value(0).toString()); // 假设显示用户名的 QLabel objectName 是 'label'
    } else {
        ui->label->setText("未知用户");
    }
}

ResetPasswordWidget::~ResetPasswordWidget()
{
    delete ui;
}

// 槽函数对应您的 “注册” (btn_register) 按钮
void ResetPasswordWidget::on_btn_register_clicked()
{
    // 1. 获取所有输入框的内容
    QString originalPass = ui->lineEdit_originalPassword->text();
    QString newPass = ui->lineEdit_password->text();
    QString confirmPass = ui->lineEdit_confirmPassword->text();

    // 2. 进行一系列输入校验
    if (originalPass.isEmpty() || newPass.isEmpty() || confirmPass.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "所有密码项都不能为空！");
        return;
    }

    if (newPass.length() < 6) {
        QMessageBox::warning(this, "密码太短", "为了安全，新密码长度不能少于6位。");
        return;
    }

    if (newPass != confirmPass) {
        QMessageBox::warning(this, "输入不一致", "两次输入的新密码不匹配，请重新输入。");
        ui->lineEdit_password->clear(); // 清空新密码框
        ui->lineEdit_confirmPassword->clear();
        ui->lineEdit_password->setFocus(); // 将光标定位到新密码框
        return;
    }

    if (newPass == originalPass) {
        QMessageBox::information(this, "提示", "新密码不能与原密码相同。");
        return;
    }

    // 3. 验证原密码是否正确
    QSqlQuery checkQuery = ODBC::query(QString("SELECT password FROM users WHERE id = %1").arg(m_userId));
    if (checkQuery.next()) {
        QString storedPassword = checkQuery.value(0).toString();
        if (storedPassword != originalPass) {
            QMessageBox::critical(this, "验证失败", "您输入的原密码不正确！");
            return;
        }
    } else {
        QMessageBox::critical(this, "系统错误", "无法验证当前用户信息，请稍后再试。");
        return;
    }

    // 4. 所有验证均通过，执行密码更新
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE users SET password = :newPass WHERE id = :id");
    updateQuery.bindValue(":newPass", newPass);
    updateQuery.bindValue(":id", m_userId);

    if (updateQuery.exec()) {
        QMessageBox::information(this, "操作成功", "密码已成功修改！");
        this->accept(); // 关闭对话框，并返回成功状态
    } else {
        QMessageBox::critical(this, "更新失败", "数据库操作失败：" + updateQuery.lastError().text());
    }
}

// 槽函数对应您的 “取消” (btn_cancel) 按钮
void ResetPasswordWidget::on_btn_cancel_clicked()
{
    this->reject(); // 关闭对话框，并返回取消状态
}
