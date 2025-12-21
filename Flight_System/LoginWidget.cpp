#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include "LoginFunc.h"
#include "UserSession.h"
#include "ODBC.h"
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

    // 强制设置第一项为用户，第二项为管理员，防止顺序错乱
    ui->comboBox->clear();
    ui->comboBox->addItem("用户");
    ui->comboBox->addItem("管理员");
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_username->setFocus();
}
void LoginWidget::on_btn_login_clicked()
{
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text();

    // 【核心修复】使用索引判断，不要用 text() == "管理员"
    // 防止因为空格、中文编码导致判断失败从而进入 else 分支
    int roleIndex = ui->comboBox->currentIndex();

    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this, "提示", "请输入用户名和密码");
        return;
    }

    if (!ODBC::connectToDB()) return;

    // --- 管理员逻辑 (索引 1) ---
    if (roleIndex == 1) {
        bool isAdmin = LoginFunc::verifyAdmin(username, password);
        if(isAdmin){
            // 管理员不需要 UserSession ID，设为 0
            UserSession::instance().setUserId(0);

            qDebug() << "管理员验证通过，发送 loginSuccessAsAdmin 信号";
            emit loginSuccessAsAdmin(); // 发送信号给 main.cpp
            this->close();
        } else {
            QMessageBox::warning(this, "失败", "管理员账号或密码错误！");
        }
    }
    // --- 用户逻辑 (索引 0) ---
    else {
        bool isUser = LoginFunc::verifyUser(username, password);
        if(isUser){
            QSqlQuery query = ODBC::query(QString("SELECT id FROM users WHERE username = '%1'").arg(username));
            if(query.next()) {
                int userId = query.value("id").toInt();
                UserSession::instance().setUserId(userId);

                emit loginSuccess(); // 发送信号给 main.cpp
                this->close();
            } else {
                QMessageBox::warning(this, "异常", "无法获取用户信息");
            }
        } else {
            QMessageBox::warning(this, "失败", "用户名或密码错误！\n(管理员请记得切换下拉框)");
        }
    }
}
void LoginWidget::on_btn_cancel_clicked()
{
    qApp->quit();
}

void LoginWidget::on_btn_register_clicked()
{
    emit goToRegister();
}
