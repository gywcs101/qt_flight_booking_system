#ifndef DATESELECTOR_H
#define DATESELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QDate>

class DateSelector : public QWidget
{
    Q_OBJECT
public:
    explicit DateSelector(QWidget *parent = nullptr);
    QDate getDate() const;
    void setDate(const QDate &date);

signals:
    void dateChanged(const QDate &date);

private:
    QComboBox *m_comboYear;
    QComboBox *m_comboMonth;
    QComboBox *m_comboDay;

    void setupUi();
    void updateMonths();
    void updateDays();
};

#endif // DATESELECTOR_H
