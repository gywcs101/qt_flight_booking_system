#include "LoginFunc.h"
#include "ODBC.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug> // 加上调试

bool LoginFunc::verifyUser(const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty()) return false;

    // 确保连接
    if (!ODBC::connectToDB()) return false;

    // 【关键】必须使用 ODBC::getDB() 获取那个命名的连接
    // 如果直接写 QSqlQuery query; 它会去找默认连接，可能导致查不到数据
    QSqlQuery query(ODBC::getDB());

    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username.trimmed());

    if (!query.exec()) {
        qDebug() << "VerifyUser SQL Failed:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        // 简单密码比对
        if (password == storedPassword) return true;
    }

    return false; // 用户名不存在或密码错误
}

// verifyAdmin 同理修改...
bool LoginFunc::verifyAdmin(const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty()) return false;
    if (!ODBC::connectToDB()) return false;

    QSqlQuery query(ODBC::getDB()); // 使用命名连接
    query.prepare("SELECT password FROM admins WHERE username = ?");
    query.addBindValue(username.trimmed());

    if (query.exec() && query.next()) {
        QString storedPassword = query.value(0).toString();
        return (password == storedPassword);
    }
    return false;
}
