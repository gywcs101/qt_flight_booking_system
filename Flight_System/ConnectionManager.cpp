#include "ConnectionManager.h"
#include <QCoreApplication>

ConnectionManager::ConnectionManager() {}

ConnectionManager::~ConnectionManager() {
    close();
}

ConnectionManager& ConnectionManager::instance() {
    static ConnectionManager instance;
    return instance;
}

bool ConnectionManager::connect() {
    // 1. 如果已经连接，直接返回
    if (m_db.isOpen()) {
        return true;
    }

    // 2. 指定使用 ODBC 驱动 (QODBC)
    // 这是 macOS 上连接 MySQL 最稳妥的方法，不需要编译插件
    m_db = QSqlDatabase::addDatabase("QODBC");

    // =================================================================
    // 3. 配置连接字符串 (Connection String)
    // =================================================================
    // 【重要】DRIVER={...} 里的名字必须和你的系统安装的一致！
    // 打开终端输入: cat /Library/ODBC/odbcinst.ini 查看具体版本号
    // 常见的有: "MySQL ODBC 8.0 Unicode Driver", "MySQL ODBC 8.4 Unicode Driver", "MySQL ODBC 9.0 Unicode Driver"
    // 下面使用的是 9.0，如果你装的是 8.x，请手动修改数字。

    QString driverName = "MySQL ODBC 8.0 Unicode Driver";

    QString dsn = QString("DRIVER={%1};"
                          "SERVER=localhost;"       // MySQL 服务器地址
                          "PORT=3306;"              // 端口
                          "DATABASE=flight_system_database;" // 数据库名 (必须先手动创建!)
                          "UID=root;"               // 你的 MySQL 用户名
                          "PWD=wayon3602;"             // 你的 MySQL 密码
                          "OPTION=3;")              // 默认选项
                      .arg(driverName);

    m_db.setDatabaseName(dsn);

    // 4. 尝试连接
    if (!m_db.open()) {
        qDebug() << "========================================";
        qDebug() << "❌ 数据库连接失败！";
        qDebug() << "错误信息：" << m_db.lastError().text();
        qDebug() << "----------------------------------------";
        qDebug() << "排查建议：";
        qDebug() << "1. 确认 MySQL 服务已启动";
        qDebug() << "2. 确认已在 MySQL 中创建了库: flight_system";
        qDebug() << "3. 确认代码里的 DRIVER 名字与 /Library/ODBC/odbcinst.ini 里的一致";
        qDebug() << "========================================";
        return false;
    }

    qDebug() << "✅ MySQL (ODBC) 连接成功！";

    // 5. 初始化表结构
    initTables();

    return true;
}

void ConnectionManager::close() {
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "数据库连接已关闭";
    }
}

bool ConnectionManager::isConnected() const {
    return m_db.isOpen();
}

void ConnectionManager::initTables() {
    if (!m_db.isOpen()) return;

    QSqlQuery query;
    // 使用 MySQL 语法: AUTO_INCREMENT
    QString sql = "CREATE TABLE IF NOT EXISTS users ("
                  "id INTEGER PRIMARY KEY AUTO_INCREMENT, "
                  "username VARCHAR(50) UNIQUE NOT NULL, "
                  "password VARCHAR(50) NOT NULL, "
                  "realname VARCHAR(50)) "
                  "CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";

    if (!query.exec(sql)) {
        qDebug() << "❌ 建表失败：" << query.lastError().text();
    } else {
        // 检查是否有数据，如果没有则插入默认管理员
        query.exec("SELECT count(*) FROM users");
        if (query.next() && query.value(0).toInt() == 0) {
            query.prepare("INSERT INTO users (username, password, realname) VALUES (?, ?, ?)");
            query.addBindValue("admin");
            query.addBindValue("123456");
            query.addBindValue("管理员");
            if(query.exec()) {
                qDebug() << "✅ 已自动创建管理员账号: admin / 123456";
            }
        }
    }
}

bool ConnectionManager::login(const QString &username, const QString &password, User &user) {
    if (!m_db.isOpen()) return false;

    QSqlQuery query;
    // 使用 prepare 防止 SQL 注入
    query.prepare("SELECT id, username, realname FROM users WHERE username = :u AND password = :p");
    query.bindValue(":u", username);
    query.bindValue(":p", password);

    if (query.exec()) {
        if (query.next()) {
            // 登录成功，填充用户信息
            user.id = query.value("id").toInt();
            user.username = query.value("username").toString();
            user.realName = query.value("realname").toString();
            qDebug() << "登录成功：" << user.username;
            return true;
        }
    } else {
        qDebug() << "查询出错：" << query.lastError().text();
    }
    return false;
}

bool ConnectionManager::registerUser(const QString &username, const QString &password, const QString &realName) {
    if (!m_db.isOpen()) return false;

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, realname) VALUES (:u, :p, :r)");
    query.bindValue(":u", username);
    query.bindValue(":p", password);
    query.bindValue(":r", realName);

    if (query.exec()) {
        qDebug() << "注册成功：" << username;
        return true;
    } else {
        // 通常失败原因：用户名重复
        qDebug() << "注册失败：" << query.lastError().text();
        return false;
    }
}
