#ifndef LOGINFUNC_H
#define LOGINFUNC_H

#include <QString>

class LoginFunc
{
public:
    static bool verifyUser(const QString &username, const QString &password);
};

#endif // LOGINFUNC_H  <--- 注意！必须放在最后一行！
