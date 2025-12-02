#ifndef HOMEFORM_H
#define HOMEFORM_H

#include <QWidget>

namespace Ui {
class homeForm;
}

class homeForm : public QWidget
{
    Q_OBJECT

public:
    explicit homeForm(QWidget *parent = nullptr);
    ~homeForm();

private:
    Ui::homeForm *ui;
};

#endif // HOMEFORM_H
