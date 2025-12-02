#ifndef USERCENTER_H
#define USERCENTER_H

#include <QWidget>

namespace Ui {
class UserCenter;
}

class UserCenter : public QWidget
{
    Q_OBJECT

public:
    explicit UserCenter(QWidget *parent = nullptr);
    ~UserCenter();

private:
    Ui::UserCenter *ui;
};

#endif // USERCENTER_H
