#include "LoginFunc.h"
#include "ODBC.h"


bool LoginFunc::verifyUser(const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty()) {
        return false;
    }

    if (!ODBC::connectToDB()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username.trimmed());

    if (!query.exec()) {
        return false;
    }

    if (!query.next()) {
        return false; // 用户不存在
    }

    QString storedPassword = query.value(0).toString();
    return (password == storedPassword);
}
