#include "ODBC.h"
#include <QDebug>
#include <QSqlError>

// 给连接起个名字，防止注销后闪退
const QString CONNECTION_NAME = "FlightSystemConnection";

bool ODBC::connectToDB()
{
    // 1. 检查是否已经连接
    if (QSqlDatabase::contains(CONNECTION_NAME)) {
        QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
        if (db.isOpen()) {
            return true;
        }
        // 尝试重新打开
        if (db.open()) return true;
    }

    // 2. 创建新连接 (使用 QODBC)
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", CONNECTION_NAME);

    // =======================================================
    // 【回归原来的方法】直接使用 DSN 名称
    // 前提：你必须在 Windows ODBC 管理器里配置过这个名字
    // =======================================================
    db.setDatabaseName("flight_system_database");

    // 填写你的数据库账号密码
    // db.setUserName("root");  // 如果DSN里没配账号，这里要取消注释
    // db.setPassword("123456"); // 如果DSN里没配密码，这里要取消注释

    // 3. 打开连接
    if (!db.open()) {
        qDebug() << "❌ 数据库连接失败:" << db.lastError().text();
        qDebug() << "请检查：1. Windows ODBC数据源是否配置正确";
        qDebug() << "        2. 数据源名称是否为 flight_system_database";
        return false;
    }

    qDebug() << "✅ 数据库连接成功 (DSN模式):" << CONNECTION_NAME;
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
    QSqlDatabase db = getDB();
    QSqlQuery sqlQuery(db); // 绑定连接

    if (!sqlQuery.exec(sql)) {
        qWarning() << "SQL Error:" << sqlQuery.lastError().text();
        qWarning() << "SQL String:" << sql;
    }
    return sqlQuery;
}
