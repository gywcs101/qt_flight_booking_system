#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>

// 用户结构体
struct User {
    int id;
    QString username;
    QString password;
    QString realName;
};

class ConnectionManager
{
public:
    static ConnectionManager& instance();

    bool connect();     // 连接数据库
    void close();       // 关闭连接
    bool isConnected() const;

    // 业务功能
    bool login(const QString &username, const QString &password, User &user);
    bool registerUser(const QString &username, const QString &password, const QString &realName = "普通用户");

private:
    ConnectionManager();
    ~ConnectionManager();

    // 禁止拷贝
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    void initTables();  // 初始化表

    QSqlDatabase m_db;
};

#endif // CONNECTIONMANAGER_H
