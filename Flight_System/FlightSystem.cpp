#include "MainWindow.h"
#include "LoginWidget.h"
#include "RegisterWidget.h"
#include "ConnectionManager.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 1. 初始化数据库
    if (!ConnectionManager::instance().connect()) {
        return -1;
    }

    // 2. 实例化所有窗口
    // 注意：这里只是创建了对象，并没有全部 show 出来
    LoginWidget w_login;
    RegisterWidget w_reg;
    MainWindow w_main;

    // 3. --- 核心逻辑连接 (信号与槽) ---

    // 场景A：从登录页 -> 跳转到注册页
    QObject::connect(&w_login, &LoginWidget::goToRegister, [&](){
        w_login.hide();      // 隐藏登录
        w_reg.show();        // 显示注册
        // 可选：w_reg.clearInputs(); // 清空注册页之前的输入
    });

    // 场景B：从注册页 -> 返回登录页
    QObject::connect(&w_reg, &RegisterWidget::goBackToLogin, [&](){
        w_reg.hide();        // 隐藏注册
        w_login.show();      // 显示登录
    });

    // 场景C：登录成功 -> 进入主界面
    QObject::connect(&w_login, &LoginWidget::loginSuccess, [&](){
        w_login.close();     // 登录页使命完成，可以关闭
        w_main.show();       // 显示主系统界面
        // 可选：w_main.refreshData(); // 让主界面加载数据
    });

    // 场景D：主界面注销 -> 返回登录 (可选功能)
    // 假设 MainWindow 里有一个 logout 信号
    /*
    QObject::connect(&w_main, &MainWindow::logout, [&](){
        w_main.close();
        w_login.show();
        w_login.clearInputs();
    });
    */

    // 4. 程序启动，先显示登录页
    w_login.show();

    return a.exec();
}
