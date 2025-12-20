#include "ODBC.h"
#include <QDebug>
#include <QSqlError>


bool ODBC::connectToDB()
{
    // 1. 检查默认连接是否已经存在
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        if (db.isOpen()) {
            return true; // 连接已存在且打开，直接返回
        } else {
            // 连接存在但关闭了，尝试重新打开
            if (db.open()) return true;
        }
    }

    // 2. 如果不存在，创建新连接
    // 注意：这里使用的是局部变量 db，但 addDatabase 会把它注册到 Qt 的全局管理中
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("flight_system_database");
    // db.setUserName("root");
    // db.setPassword("123456");

    if (!db.open()) {
        qDebug() << "数据库连接失败:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ 成功连接到数据库!";
    return true;
}

// [新增] 辅助函数，安全获取数据库连接
QSqlDatabase ODBC::getDB() {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        return QSqlDatabase::database("qt_sql_default_connection");
    } else {
        // 如果连接丢了，尝试重连
        connectToDB();
        return QSqlDatabase::database("qt_sql_default_connection");
    }
}

QSqlQuery ODBC::query(const QString &sql)
{
    // 1. 获取连接
    QSqlDatabase db = getDB();

    // 2. 再次检查是否打开
    if (!db.isOpen()) {
        qWarning() << "数据库未连接，正在尝试重新连接...";
        if (!connectToDB()) {
            qWarning() << "重新连接失败，无法执行查询!";
            return QSqlQuery(); // 返回无效查询对象
        }
        db = getDB(); // 刷新 db 对象
    }

    // 3. 创建查询对象 (传入 db 确保使用正确的连接)
    QSqlQuery sqlQuery(db);

    // 4. 执行
    if (!sqlQuery.exec(sql)) {
        qWarning() << "SQL执行失败:" << sqlQuery.lastError().text();
        qWarning() << "失败的SQL语句:" << sql;
    }

    return sqlQuery;
}


