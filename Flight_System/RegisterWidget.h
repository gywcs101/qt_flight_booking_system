#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>

// [核心修正] 向前声明正确的 Ui 类
namespace Ui {
class RegisterWidget;
}

class RegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();

signals:
    void goBackToLogin(); // 返回登录信号

private slots:
    void on_btn_register_clicked();
    void on_btn_cancel_clicked();

private:
    // [核心修正] 声明正确的 ui 指针类型
    Ui::RegisterWidget *ui;
};

#endif // REGISTERWIDGET_H
