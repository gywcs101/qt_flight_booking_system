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
     * @brief 加载当前登录用户的数据
     * @param userId 用户的ID (对应数据库表中的 id 字段)
     */
    void loadUserData(QString userId);

signals:
    // 发送退出登录信号，通知主窗口切换页面
    void logoutSignal();

private slots:
    void onBtnEditClicked();   // 槽函数：编辑/保存按钮
    void onBtnPassClicked();   // 槽函数：修改密码按钮
    void onBtnLogoutClicked(); // 槽函数：退出登录按钮

private:
    Ui::UserCenter *ui;

    bool isEditing;         // 标记当前是否处于编辑模式
    QString m_currentUserId; // 存储当前用户的ID，用于更新数据时做 WHERE 条件

    // 辅助函数：设置输入框是否可编辑
    void setInputsEnabled(bool enable);

    // 辅助函数：处理圆形头像显示
    void updateAvatarDisplay(QString path);
};

#endif // USERCENTER_H
