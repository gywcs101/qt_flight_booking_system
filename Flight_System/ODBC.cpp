#include "ODBC.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

bool ODBC::connectToDB()
{
    if (QSqlDatabase::database().isValid() && QSqlDatabase::database().isOpen()) {
        return true;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("flight_system_database");

    if (!db.open()) {
        qDebug() << "数据库连接失败:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ 成功连接到 flight_system_database via ODBC!";
    return true;
}
