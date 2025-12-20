#ifndef ODBC_H
#define ODBC_H

#include <QSqlDatabase>
#include <QSqlQuery>

class ODBC
{
public:
    // 连接数据库
    static bool connectToDB();

    // [核心] 获取数据库实例的统一接口
    static QSqlDatabase getDB();

    // 执行查询的通用接口
    static QSqlQuery query(const QString &sql);
};

#endif // ODBC_H
