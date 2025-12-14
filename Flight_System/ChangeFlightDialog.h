#ifndef CHANGEFLIGHTDIALOG_H
#define CHANGEFLIGHTDIALOG_H

#include <QDialog>
#include "FlightData.h"

namespace Ui { class ChangeFlightDialog; }

class ChangeFlightDialog : public QDialog
{
    Q_OBJECT

public:
    // 参数：父对象，旧航班ID，出发地，目的地，旧票价
    explicit ChangeFlightDialog(QWidget *parent, QString oldFlightId, QString dep, QString arr, double oldPrice);
    ~ChangeFlightDialog();

    // 返回用户选择的新航班（如果没选则为空）
    FlightData getSelectedFlight() const { return m_selectedFlight; }
    bool isConfirmed() const { return m_confirmed; }

private:
    Ui::ChangeFlightDialog *ui;
    QString m_oldFlightId;
    double m_oldPrice;
    FlightData m_selectedFlight;
    bool m_confirmed = false;

    void loadAlternativeFlights(QString dep, QString arr);
    void confirmChange(const FlightData &newFlight); // 弹出确认框
};

#endif // CHANGEFLIGHTDIALOG_H
