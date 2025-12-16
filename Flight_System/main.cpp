#include "MainWindow.h"
#include "LoginWidget.h"
#include "RegisterWidget.h"
#include "AdminWidget.h"      // [核心修改] 包含 AdminWidget 的头文件
#include <QApplication>
#include <QFile>
#include <QDebug>
#include "ODBC.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载样式表
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString style = QString::fromUtf8(file.readAll());
        a.setStyleSheet(style);
        file.close();
    } else {
        qDebug() << "样式表打开失败，请检查路径";
    }

    // 连接数据库
    if (!ODBC::connectToDB()) {
        qDebug() << "数据库连接失败，程序退出";
        return -1;
    }

    // --- 创建所有需要的窗口实例 ---
    LoginWidget w_login;
    RegisterWidget w_reg;
    MainWindow w_main;
    AdminWidget w_admin; // [核心修改] 创建 AdminWidget 实例

    // --- 建立所有窗口之间的跳转逻辑 ---

    // 1. 从登录页 -> 注册页
    QObject::connect(&w_login, &LoginWidget::goToRegister, [&](){
        w_login.hide();
        w_reg.show();
    });

    // 2. 从注册页 -> 登录页
    QObject::connect(&w_reg, &RegisterWidget::goBackToLogin, [&](){
        w_reg.hide();
        w_login.show();
    });

    // 3. (用户)登录成功 -> 主窗口
    QObject::connect(&w_login, &LoginWidget::loginSuccess, [&](){
        // w_login 已经在其内部 close() 了
        w_main.show();
    });

    // 4. [核心修改] (管理员)登录成功 -> 管理员后台窗口
    QObject::connect(&w_login, &LoginWidget::loginSuccessAsAdmin, [&](){
        // w_login 已经在其内部 close() 了
        w_admin.show();
    });

    // 程序启动时，首先显示登录窗口
    w_login.show();

    return a.exec();
}
