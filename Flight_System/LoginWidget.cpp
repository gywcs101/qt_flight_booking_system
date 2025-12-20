#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include "LoginFunc.h"
#include "UserSession.h"
#include "ODBC.h" // 【重要】引入 ODBC 确保连接
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

    // 可以在这里预先连接一次数据库，或者在点击按钮时连接
    // ODBC::connectToDB();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_btn_login_clicked()
{
    // 1. 获取输入
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text();
    QString role = ui->comboBox->currentText();

    // 2. 非空校验
    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this, "提示", "请输入用户名和密码");
        return;
    }

    // 3. 【关键修复】确保数据库连接正常
    // 如果之前断开了，这里会重新连接。如果没有这一步，直接查库可能会崩。
    if (!ODBC::connectToDB()) {
        QMessageBox::critical(this, "错误", "无法连接到数据库，请检查网络或配置！");
        return;
    }

    // 4. 分角色登录逻辑
    if(role == "管理员"){
        // 验证管理员
        bool isAdmin = LoginFunc::verifyAdmin(username, password);
        if(isAdmin){
            qDebug() << "管理员登录成功:" << username;

            // 管理员不需要记录 UserSession ID (或者设为0/特殊值)
            // UserSession::instance().setUserId(0);

            emit loginSuccessAsAdmin();
            this->close();
        } else {
            QMessageBox::warning(this, "登录失败", "管理员账号或密码错误！");
        }
    }
    else {
        // 验证普通用户
        bool isUser = LoginFunc::verifyUser(username, password);

        if(isUser){
            // 【关键】指定连接
            QSqlQuery query(ODBC::getDB());

            query.prepare("SELECT id FROM users WHERE username = :u");
            query.bindValue(":u", username);

            if(query.exec() && query.next()) {
                int userId = query.value("id").toInt();
                UserSession::instance().setUserId(userId);
                qDebug() << "用户登录成功，ID:" << userId;
            } else {
                // 极少见的情况：验证通过但查不到ID
                qWarning() << "警告：无法获取用户ID";
                UserSession::instance().setUserId(-1);
            }

            // 发送信号并关闭登录窗
            emit loginSuccess();
            this->close();
        }
        else{
            QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
        }
    }
}

void LoginWidget::on_pushButton_2_clicked()
{
    // 退出程序
    qApp->quit();
}

void LoginWidget::on_btn_register_clicked()
{
    // 跳转注册
    emit goToRegister();
}
