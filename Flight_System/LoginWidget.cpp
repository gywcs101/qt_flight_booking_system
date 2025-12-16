#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include "LoginFunc.h"
#include "UserSession.h"
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QSqlQuery>
#include <QSqlError>

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
            // [核心修改] 管理员登录成功
            qDebug() << "管理员登录成功:" << username;

            // 1. 发射管理员登录成功信号
            emit loginSuccessAsAdmin();

            // 2. 关闭当前登录窗口
            this->close();

        } else {
            QMessageBox::warning(this, "登录失败", "管理员账号或密码错误！");
        }
    }
    else { // 用户登录
        bool isUser = LoginFunc::verifyUser(username, password);
        if(isUser){
            QSqlQuery query;
            query.prepare("SELECT id FROM users WHERE username = :u AND password = :p");
            query.bindValue(":u", username);
            query.bindValue(":p", password);

            if(query.exec() && query.next()) {
                int userId = query.value("id").toInt();
                UserSession::instance().setUserId(userId);
                qDebug() << "登录成功，当前用户ID:" << userId;
            } else {
                qDebug() << "警告：验证通过但未查询到ID，数据库可能异常";
            }

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
