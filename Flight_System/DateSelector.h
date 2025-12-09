#ifndef DATESELECTOR_H
#define DATESELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QDate>

class DateSelector : public QWidget
{
    Q_OBJECT
public:
    explicit DateSelector(QWidget *parent = nullptr);

    // 获取当前选中的日期
    QDate getDate() const;

    // 设置显示日期
    void setDate(const QDate &date);

signals:
    // 当用户改变日期时触发
    void dateChanged(const QDate &date);

private:
    QLabel *m_lblYear;      // 显示年份
    QComboBox *m_comboMonth;// 月份下拉框
    QComboBox *m_comboDay;  // 日期下拉框

    void setupUi();         // 初始化界面
    void updateDays();      // 根据年月计算天数
};

#endif // DATESELECTOR_H
