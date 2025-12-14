#ifndef RESETPASSWORDWIDGET_H
#define RESETPASSWORDWIDGET_H

#include <QDialog>

namespace Ui {
class ResetPasswordWidget;
}

class ResetPasswordWidget : public QDialog
{
    Q_OBJECT

public:
    // 构造函数接收当前用户的ID，以便进行密码验证和更新
    explicit ResetPasswordWidget(int userId, QWidget *parent = nullptr);
    ~ResetPasswordWidget();

private slots:
    void on_btnConfirm_clicked(); // “确认修改”按钮的槽函数
    void on_btnCancel_clicked();  // “取消”按钮的槽函数

private:
    Ui::ResetPasswordWidget *ui;
    int m_userId; // 存储当前用户的ID
};

#endif // RESETPASSWORDWIDGET_H
