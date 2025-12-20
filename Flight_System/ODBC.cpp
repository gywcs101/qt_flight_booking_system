#include "ODBC.h"
#include <QDebug>
#include <QSqlError>

// 定义唯一的连接名称，防止混淆
const QString CONNECTION_NAME = "FlightSystemConnection";

bool ODBC::connectToDB()
{
    // 1. 检查连接是否已存在
    if (QSqlDatabase::contains(CONNECTION_NAME)) {
        QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
        if (db.isOpen()) {
            return true;
        }
        // 存在但关闭了，尝试重新打开
        if (db.open()) return true;
    }

    // 2. 创建新连接 (指定连接名，不使用默认连接)
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", CONNECTION_NAME);
    db.setDatabaseName("flight_system_database"); // 你的数据源名称

    if (!db.open()) {
        qDebug() << "数据库连接失败:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ 数据库连接成功:" << CONNECTION_NAME;
    return true;
}

QSqlDatabase ODBC::getDB()
{
    if (QSqlDatabase::contains(CONNECTION_NAME)) {
        return QSqlDatabase::database(CONNECTION_NAME);
    } else {
        connectToDB();
        return QSqlDatabase::database(CONNECTION_NAME);
    }
}

QSqlQuery ODBC::query(const QString &sql)
{
    // [核心修复] 创建 Query 时显式传入我们的数据库连接
    // 否则它会去抓默认连接，导致第二次登录时找不到驱动而闪退
    QSqlDatabase db = getDB();
    QSqlQuery sqlQuery(db);

    if (!sqlQuery.exec(sql)) {
        qWarning() << "SQL执行失败:" << sqlQuery.lastError().text();
        qWarning() << "SQL内容:" << sql;
    }
    return sqlQuery;
}
