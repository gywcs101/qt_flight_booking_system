#ifndef USERCENTER_H
#define USERCENTER_H

#include <QWidget>
#include <QDate>

namespace Ui {
class UserCenter;
}

class UserCenter : public QWidget
{
    Q_OBJECT

public:
    explicit UserCenter(QWidget *parent = nullptr);
    ~UserCenter();

    /**
     * @brief [核心] 加载并显示当前登录用户的数据。
     *        此函数会从 UserSession 获取用户ID。
     */
    void loadCurrentUserData();

signals:
    // 发送退出登录信号，通知主窗口切换页面
    void logoutSignal();

private slots:
    void onBtnEditClicked();   // 槽函数：编辑/保存按钮
    void onBtnPassClicked();   // 槽函数：修改密码按钮
    void onBtnLogoutClicked(); // 槽函数：退出登录按钮

private:
    Ui::UserCenter *ui;

    bool isEditing;      // 标记当前是否处于编辑模式
    int m_currentUserId; // 存储当前用户的ID (int类型)

    void setInputsEnabled(bool enable);
    void updateAvatarDisplay(const QString& path);
};

#endif // USERCENTER_H
