#ifndef ODBC_H
#define ODBC_H

#include <QSqlDatabase>
#include <QSqlQuery>

// 声明全局数据库对象
extern QSqlDatabase db;

class ODBC
{
public:
    /**
     * @brief [名称已统一] 建立与数据库的连接
     * @return true 连接成功, false 连接失败
     */
    static bool connectToDB();

    /**
     * @brief [名称已统一] 执行一条SQL查询语句
     * @param sql 要执行的SQL语句
     * @return 返回QSqlQuery对象，用于获取结果或检查错误
     */
    static QSqlQuery query(const QString &sql);
};

#endif // ODBC_H
