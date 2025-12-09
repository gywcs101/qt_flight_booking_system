#ifndef USERSESSION_H
#define USERSESSION_H

class UserSession {
public:
    static UserSession& instance() {
        static UserSession instance;
        return instance;
    }

    void setUserId(int id) { m_userId = id; }
    int getUserId() const { return m_userId; }

private:
    UserSession() : m_userId(-1) {} // 默认 -1
    int m_userId;
};

#endif // USERSESSION_H
