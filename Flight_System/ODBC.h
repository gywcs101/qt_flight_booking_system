#ifndef ODBC_H
#define ODBC_H

#include <QSqlDatabase>
#include <QSqlQuery>

// 【重要】删除了 extern QSqlDatabase db;  <-- 绝对不能有这个

class ODBC
{
public:
    static bool connectToDB();
    static QSqlQuery query(const QString &sql);

    // 辅助函数：获取当前连接
    static QSqlDatabase getDB();
};

#endif // ODBC_H
