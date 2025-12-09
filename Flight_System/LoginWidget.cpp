#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include "LoginFunc.h"
#include "UserSession.h" // 【新增】引入用户会话管理
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QSqlQuery>     // 【新增】用于查询ID
#include <QSqlError>     // 【新增】

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_btn_login_clicked()
{
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text();
    QString role = ui->comboBox->currentText();

    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this,"错误输入","别忘了填写用户名和密码哦");
        return;
    }

    if(role == "管理员"){
        bool isAdmin = LoginFunc::verifyAdmin(username, password);
        if(isAdmin){
            // 管理员不需要记录到 UserSession (或者你可以设为0)
            // UserSession::instance().setUserId(0);

            QMessageBox::information(this, "提示", "管理员登录成功！\n(后台管理系统开发中...)");
        } else {
            QMessageBox::warning(this, "登录失败", "管理员账号或密码错误！");
        }
    }
    else{
        // 1. 先验证账号密码是否正确
        bool isUser = LoginFunc::verifyUser(username, password);

        if(isUser){
            // =======================================================
            // 【核心修改】登录成功后，查询并保存用户 ID
            // =======================================================
            QSqlQuery query;
            // 根据用户名和密码查询 ID (确保唯一性)
            query.prepare("SELECT id FROM users WHERE username = :u AND password = :p");
            query.bindValue(":u", username);
            query.bindValue(":p", password);

            if(query.exec() && query.next()) {
                int userId = query.value("id").toInt();

                // 将 ID 存入全局单例，供收藏功能使用
                UserSession::instance().setUserId(userId);

                qDebug() << "登录成功，当前用户ID:" << userId;
            } else {
                qDebug() << "警告：验证通过但未查询到ID，数据库可能异常";
                // 可以在这里处理异常，或者默认给个 -1
            }
            // =======================================================

            // 发送信号切换窗口
            emit loginSuccess();
            this->close();
        }
        else{
            QMessageBox::warning(this,"登录失败","用户名或密码有误！");
        }
    }
}

void LoginWidget::on_pushButton_2_clicked()
{
    qApp->quit();
}

void LoginWidget::on_btn_register_clicked()
{
    emit goToRegister();
}
