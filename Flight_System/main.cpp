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

    // [核心修改] 防止主窗口隐藏、登录窗口还没显示的那一瞬间程序自动退出
    a.setQuitOnLastWindowClosed(false);

    // 加载样式表
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString style = QString::fromUtf8(file.readAll());
        a.setStyleSheet(style);
        file.close();
    }

    // 连接数据库
    if (!ODBC::connectToDB()) {
        qDebug() << "数据库连接失败，程序退出";
        return -1;
    }

    // --- 创建所有需要的窗口实例 ---
    // 注意：这些窗口在程序运行期间一直存在，只是显示或隐藏
    LoginWidget w_login;
    RegisterWidget w_reg;
    MainWindow w_main;
    AdminWidget w_admin;

    // --- 建立窗口跳转逻辑 ---

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
        w_login.hide(); // 只是隐藏，不要 close，方便下次注销回来复用
        w_main.show();
    });

    // 4. (管理员)登录成功 -> 后台
    QObject::connect(&w_login, &LoginWidget::loginSuccessAsAdmin, [&](){
        w_login.hide();
        w_admin.show();
    });

    // 5. [核心新增] 主窗口注销 -> 回到登录
    QObject::connect(&w_main, &MainWindow::logout, [&](){
        w_main.hide();  // 隐藏主窗口
        w_login.show(); // 重新显示最开始那个 w_login 实例
        // 这里的 w_login 和上面的 w_login 是同一个对象，所以信号槽连接依然有效！
    });

    // 启动
    w_login.show();

    return a.exec();
}
