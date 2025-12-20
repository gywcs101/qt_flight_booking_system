#include "ODBC.h"
#include <QDebug>
#include <QSqlError>

// 【重要】删除了 QSqlDatabase db; 全局变量

bool ODBC::connectToDB()
{
    // 1. 检查默认连接是否已存在
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        if (db.isOpen()) {
            return true;
        }
        // 如果存在但没打开，尝试打开
        if (db.open()) return true;
    }

    // 2. 如果不存在，创建新连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC"); // 使用默认连接名
    db.setDatabaseName("flight_system_database");

    if (!db.open()) {
        qDebug() << "数据库连接失败:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ 数据库连接成功 (ODBC)";
    return true;
}

// 获取数据库连接的辅助函数
QSqlDatabase ODBC::getDB() {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        return QSqlDatabase::database("qt_sql_default_connection");
    } else {
        // 如果连接丢了，重新连接
        connectToDB();
        return QSqlDatabase::database("qt_sql_default_connection");
    }
}

QSqlQuery ODBC::query(const QString &sql)
{
    // 1. 获取连接 (局部变量，用完即焚)
    QSqlDatabase db = getDB();

    // 2. 双重检查
    if (!db.isOpen()) {
        if (!connectToDB()) {
            return QSqlQuery(); // 返回无效对象
        }
        db = getDB(); // 刷新
    }

    // 3. 创建查询 (关联到 db)
    QSqlQuery sqlQuery(db);

    // 4. 执行
    if (!sqlQuery.exec(sql)) {
        qWarning() << "SQL Error:" << sqlQuery.lastError().text();
        qWarning() << "SQL String:" << sql;
    }

    return sqlQuery;
}
