#ifndef RESETPASSWORDWIDGET_H
#define RESETPASSWORDWIDGET_H

#include <QDialog>

// --- [核心修改 1] ---
// 移除不完整的向前声明，因为它会导致 “incomplete type” 错误
// namespace Ui { class ResetPasswordWidget; }

// --- [核心修改 2] ---
// 直接包含由 .ui 文件生成的头文件
// 这个头文件内部包含了完整的 Ui::ResetPasswordWidget 类的定义
#include "ui_ResetPasswordWidget.h"

class ResetPasswordWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ResetPasswordWidget(int userId, QWidget *parent = nullptr);
    ~ResetPasswordWidget();

private slots:
    // --- [核心修改 3] ---
    // 在这里声明您在 .cpp 文件中实现的两个槽函数
    // 这样编译器就知道这个类确实有这两个成员函数
    void on_btn_confirm_clicked();
    void on_btn_cancel_clicked();

private:
    // 现在 ui 指针的类型是完整的，而不是不完整的
    Ui::ResetPasswordWidget *ui;
    int m_userId;
};

#endif // RESETPASSWORDWIDGET_H
