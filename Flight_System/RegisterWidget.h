#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>

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
    Ui::RegisterWidget *ui;
};

#endif // REGISTERWIDGET_H
