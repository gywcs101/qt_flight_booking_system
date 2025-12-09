#ifndef ALLFLIGHTSPAGE_H
#define ALLFLIGHTSPAGE_H

#include <QWidget>
#include "DateSelector.h" // <--- 加入这个

namespace Ui {
class AllFlightsPage;
}

class AllFlightsPage : public QWidget
{
    Q_OBJECT

public:
    explicit AllFlightsPage(QWidget *parent = nullptr);
    ~AllFlightsPage();

    void loadFlightsData(); // 加载数据函数

private:
    Ui::AllFlightsPage *ui;
};

#endif // ALLFLIGHTSPAGE_H
