#ifndef ALLFLIGHTSPAGE_H
#define ALLFLIGHTSPAGE_H

#include <QWidget>
#include "DateSelector.h" // 确保包含

namespace Ui { class AllFlightsPage; }

class AllFlightsPage : public QWidget
{
    Q_OBJECT
public:
    explicit AllFlightsPage(QWidget *parent = nullptr);
    ~AllFlightsPage();
    void loadFlightsData();

private:
    Ui::AllFlightsPage *ui;
};

#endif // ALLFLIGHTSPAGE_H
