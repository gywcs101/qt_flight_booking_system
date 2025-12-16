#ifndef ABOUTPAGE_H
#define ABOUTPAGE_H

#include <QWidget>

namespace Ui {
class AboutPage;
}

class AboutPage : public QWidget
{
    Q_OBJECT

public:
    explicit AboutPage(QWidget *parent = nullptr);
    ~AboutPage();

private:
    Ui::AboutPage *ui;

    // 【修改】去掉了 role 参数，只传名字和图片路径
    QWidget* createDevProfile(QString name, QString imagePath);
};

#endif // ABOUTPAGE_H
