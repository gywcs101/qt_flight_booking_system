#ifndef ODBC_H
#define ODBC_H

#include <QSqlDatabase>
#include <QSqlQuery>

class ODBC
{
public:
    /**
     * @brief 建立与数据库的连接
     * @return true 连接成功, false 连接失败
     */
    static bool connectToDB();

    /**
     * @brief 执行一条SQL查询语句
     * @param sql 要执行的SQL语句
     * @return 返回QSqlQuery对象
     */
    static QSqlQuery query(const QString &sql);

    // [新增] 获取当前数据库连接的辅助函数
    static QSqlDatabase getDB();
};

#endif // ODBC_H
