#include "ODBC.h"
#include <QDebug>
#include <QSqlError>

// 定义全局数据库对象的实体
QSqlDatabase db;

/**
 * @brief 建立与数据库的连接 (保留您原来的函数名)
 * @return true 连接成功, false 连接失败
 */
bool ODBC::connectToDB()
{
    // 检查默认连接是否已存在且打开
    if (QSqlDatabase::contains("qt_sql_default_connection") && QSqlDatabase::database("qt_sql_default_connection").isOpen()) {
        db = QSqlDatabase::database("qt_sql_default_connection");
        return true;
    }

    // 添加数据库驱动
    db = QSqlDatabase::addDatabase("QODBC");

    // 设置你在ODBC数据源管理器中配置好的DSN名称
    db.setDatabaseName("flight_system_database");
    // db.setUserName("your_username");
    // db.setPassword("your_password");

    if (!db.open()) {
        qDebug() << "数据库连接失败:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ 成功连接到 flight_system_database via ODBC!";
    return true;
}

/**
 * @brief [补全] 执行一条SQL查询语句
 * @param sql 要执行的SQL语句
 * @return 返回QSqlQuery对象，用于获取结果或检查错误
 */
QSqlQuery ODBC::query(const QString &sql)
{
    // 确保数据库是连接状态，如果不是，尝试连接
    if (!db.isOpen()) {
        qWarning() << "数据库未连接，正在尝试重新连接...";
        if (!connectToDB()) { // 调用您自己的连接函数
            qWarning() << "重新连接失败，无法执行查询!";
            return QSqlQuery(); // 返回一个无效的query对象
        }
    }

    // 创建一个查询对象，它会自动使用全局的db连接
    QSqlQuery sqlQuery(db);

    // 执行查询
    if (!sqlQuery.exec(sql)) {
        qWarning() << "SQL执行失败:" << sqlQuery.lastError().text();
        qWarning() << "失败的SQL语句:" << sql;
    }

    return sqlQuery;
}
