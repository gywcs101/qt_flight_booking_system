#ifndef ALLFLIGHTSPAGE_H
#define ALLFLIGHTSPAGE_H

#include <QWidget>

// 向前声明，避免不必要的头文件包含
namespace Ui { class AllFlightsPage; }
class DateSelector;

class AllFlightsPage : public QWidget
{
    Q_OBJECT

public:
    explicit AllFlightsPage(QWidget *parent = nullptr);
    ~AllFlightsPage();

public slots:
    // 将其声明为 public slot，既可以被外部调用，也可以连接信号
    void loadFlightsData();

private:
    Ui::AllFlightsPage *ui;
};

#endif // ALLFLIGHTSPAGE_H
