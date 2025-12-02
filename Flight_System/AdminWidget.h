#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>

// 前置声明，避免包含过多头文件
namespace Ui { class AdminWidget; }

class AdminWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdminWidget(QWidget *parent = nullptr);
    ~AdminWidget();

private:
    Ui::AdminWidget *ui;
};

#endif // ADMINWIDGET_H
