#include "LoginWidget.h"
#include "ui_LoginWidget.h" // 注意这里自动生成的头文件名变了
#include "LoginFunc.h"
#include <QMessageBox>
#include <QDebug>

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

    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this,"错误输入","别忘了填写用户名和密码哦");
        return;
    }

    // 调用搬过来的 LoginFunc
    bool result = LoginFunc::verifyUser(username, password);
    
    if(result){
        // QMessageBox::information(this,"欢迎","登录成功"); 
        // 登录成功后，不要只弹窗，要发送信号给 Main.cpp 切换窗口
        emit loginSuccess(); 
        this->close(); // 关闭登录窗
    }
    else{
        QMessageBox::warning(this,"登录失败","用户名或密码有误！");
    }
}