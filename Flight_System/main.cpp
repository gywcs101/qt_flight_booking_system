#include "MainWindow.h"
#include "LoginWidget.h"
#include "RegisterWidget.h"
#include "AdminWidget.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include "ODBC.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 【关键修复 1】防止窗口切换间隙程序自动退出
    // 当主窗口隐藏、登录窗还没显示的那一瞬间，如果没有这句，程序就结束了
    a.setQuitOnLastWindowClosed(false);

    // 加载样式表
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString style = QString::fromUtf8(file.readAll());
        a.setStyleSheet(style);
        file.close();
    }

    if (!ODBC::connectToDB()) {
        return -1;
    }

    LoginWidget w_login;
    RegisterWidget w_reg;
    MainWindow w_main;
    AdminWidget w_admin;

    // --- 信号连接 ---

    // 1. 登录 -> 注册
    QObject::connect(&w_login, &LoginWidget::goToRegister, [&](){
        w_login.hide();
        w_reg.show();
    });

    // 2. 注册 -> 登录
    QObject::connect(&w_reg, &RegisterWidget::goBackToLogin, [&](){
        w_reg.hide();
        w_login.show();
    });

    // 3. (用户)登录成功 -> 主窗口
    QObject::connect(&w_login, &LoginWidget::loginSuccess, [&](){
        w_login.hide();
        w_main.show();
    });

    // 4. (管理员)登录成功 -> 管理员后台
    QObject::connect(&w_login, &LoginWidget::loginSuccessAsAdmin, [&](){
        qDebug() << "【Main检测】收到管理员信号 -> 正在打开 w_admin";
        w_login.hide();
        w_admin.show();
        w_admin.setWindowTitle("【管理员后台管理系统】"); // 强制改标题，方便你区分
    });

    // 5. 【关键修复 2】主窗口注销 -> 回到登录
    // 你之前的代码里漏了这一段，所以退出后没反应或无法重登
    QObject::connect(&w_main, &MainWindow::logout, [&](){
        w_main.hide();  // 隐藏主窗口
        w_login.show(); // 重新显示登录窗口（这会触发 LoginWidget 的 showEvent 清空输入框）
    });

    // 启动
    w_login.show();

    return a.exec();
}
