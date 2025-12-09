#ifndef ALLFLIGHTSPAGE_H
#define ALLFLIGHTSPAGE_H

#include <QWidget>

namespace Ui {
class AllFlightsPage;
}

class AllFlightsPage : public QWidget
{
    Q_OBJECT

public:
    explicit AllFlightsPage(QWidget *parent = nullptr);
    ~AllFlightsPage();

private:
    Ui::AllFlightsPage *ui;
};

#endif // ALLFLIGHTSPAGE_H
