#include "MainWindow.h"
#include "LoginWidget.h"
#include "RegisterWidget.h"
#include <QApplication>
#include <QFile>    // 【新增】必须加这个，否则报错 QFile unknown
#include <QDebug>   // 【新增】必须加这个，否则报错 qDebug unknown
#include "ODBC.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载样式表 (这段是你截图里的代码，我帮你补全了头文件)
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString style = QString::fromUtf8(file.readAll());
        a.setStyleSheet(style);
        file.close();
    } else {
        qDebug() << "样式表打开失败，请检查路径";
    }

    // --- 下面是之前的逻辑连接代码 ---
    if (!ODBC::connectToDB()) {                      // <-- 换成这行
        qDebug() << "数据库连接失败，程序退出";
        return -1;
    }

    LoginWidget w_login;
    RegisterWidget w_reg;
    MainWindow w_main;

    // 连接信号槽
    QObject::connect(&w_login, &LoginWidget::goToRegister, [&](){
        w_login.hide();
        w_reg.show();
    });
    QObject::connect(&w_reg, &RegisterWidget::goBackToLogin, [&](){
        w_reg.hide();
        w_login.show();
    });
    QObject::connect(&w_login, &LoginWidget::loginSuccess, [&](){
        w_login.close();
        w_main.show();
    });

    w_login.show();
    return a.exec();
}
