#ifndef USERCENTER_H
#define USERCENTER_H

#include <QWidget>
#include <QDate>
#include <QEvent>      // [新增]
#include <QFileDialog> // [新增]

namespace Ui {
class UserCenter;
}

class UserCenter : public QWidget
{
    Q_OBJECT

public:
    explicit UserCenter(QWidget *parent = nullptr);
    ~UserCenter();

    void loadCurrentUserData();

signals:
    void logoutSignal();

protected:
    // [新增] 重写事件过滤器，用于捕获Label的点击事件
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onBtnEditClicked();
    void onBtnPassClicked();
    void onBtnLogoutClicked();

private:
    Ui::UserCenter *ui;

    bool isEditing;
    int m_currentUserId;

    void setInputsEnabled(bool enable);
    void updateAvatarDisplay(const QString& path);

    // [新增] 处理更换头像的具体逻辑
    void handleAvatarChange();
};

#endif // USERCENTER_H
