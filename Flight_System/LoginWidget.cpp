#include "LoginWidget.h"
#include "ui_LoginWidget.h" // 注意这里自动生成的头文件名变了
#include "LoginFunc.h"
#include <QMessageBox>
#include <QDebug>
#include <QApplication>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground); // 也就是旧代码里的设置
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

// --- 这里是把 widget.cpp 的逻辑搬过来，并把 Widget:: 改为 LoginWidget:: ---

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
            // 成功：只弹窗，不跳转
            QMessageBox::information(this, "提示", "管理员登录成功！\n(后台管理系统开发中...)");
        } else {
            // 失败：通用报错
            QMessageBox::warning(this, "登录失败", "管理员账号或密码错误！");
        }
    }
    else{
        // 调用搬过来的 LoginFunc
        bool isUser = LoginFunc::verifyUser(username, password);

        if(isUser){
            // QMessageBox::information(this,"欢迎","登录成功");
            // 登录成功后，不要只弹窗，要发送信号给 Main.cpp 切换窗口
            emit loginSuccess();
            this->close(); // 关闭登录窗
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

