#ifndef ODBC_H
#define ODBC_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class ODBC
{
public:
    // 连接数据库
    static bool connectToDB();

    // 获取数据库连接实例
    static QSqlDatabase getDB();

    // 执行查询的通用接口
    static QSqlQuery query(const QString &sql);

    // 获取最后一次错误信息
    static QString getLastError();
};

#endif // ODBC_H
