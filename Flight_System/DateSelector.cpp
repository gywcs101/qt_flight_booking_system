#include "DateSelector.h"
#include <QHBoxLayout>
#include <QListView>

DateSelector::DateSelector(QWidget *parent) : QWidget(parent)
{
    setupUi();

    // 1. 初始化年份 (今年和明年)
    int curYear = QDate::currentDate().year();
    m_comboYear->addItem(QString::number(curYear) + "年", curYear);
    m_comboYear->addItem(QString::number(curYear + 1) + "年", curYear + 1);

    // 2. 【关键】手动触发一次更新，确保月份和日期框被填满
    // 否则 getDate() 会因为下拉框为空而返回错误日期
    updateMonths();

    // 3. 选中今天
    setDate(QDate::currentDate());

    // 4. 建立信号连接
    connect(m_comboYear, &QComboBox::currentIndexChanged, this, &DateSelector::updateMonths);
    connect(m_comboMonth, &QComboBox::currentIndexChanged, this, &DateSelector::updateDays);
    connect(m_comboDay, &QComboBox::currentIndexChanged, [this](){
        if(m_comboDay->count() > 0) emit dateChanged(getDate());
    });
}

void DateSelector::setupUi()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    QString comboStyle = R"(
        QComboBox {
            border: 1px solid #DCDFE6; border-radius: 4px; padding: 4px;
            background: white; min-width: 60px; font-family: 'Microsoft YaHei';
        }
        QComboBox:hover { border: 1px solid #409EFF; }
        QComboBox::drop-down { border: none; width: 20px; }
        QComboBox QAbstractItemView { outline: none; selection-background-color: #409EFF; }
    )";

    m_comboYear = new QComboBox; m_comboYear->setView(new QListView);
    m_comboMonth = new QComboBox; m_comboMonth->setView(new QListView);
    m_comboDay = new QComboBox; m_comboDay->setView(new QListView);

    m_comboYear->setStyleSheet(comboStyle);
    m_comboMonth->setStyleSheet(comboStyle);
    m_comboDay->setStyleSheet(comboStyle);

    layout->addWidget(m_comboYear);
    layout->addWidget(m_comboMonth);
    layout->addWidget(m_comboDay);

    // 强制大小，防止被压缩
    this->setMinimumSize(240, 40);
}

void DateSelector::updateMonths()
{
    m_comboMonth->blockSignals(true);
    m_comboMonth->clear();

    QDate today = QDate::currentDate();
    int selYear = m_comboYear->currentData().toInt();

    // 逻辑：如果是今年，从本月开始；明年则显示1-12月
    int startMonth = (selYear == today.year()) ? today.month() : 1;

    for (int i = startMonth; i <= 12; i++) {
        m_comboMonth->addItem(QString("%1月").arg(i, 2, 10, QChar('0')), i);
    }
    m_comboMonth->blockSignals(false);

    // 月份变了，必须刷新天数
    updateDays();
}

void DateSelector::updateDays()
{
    m_comboDay->blockSignals(true);
    m_comboDay->clear();

    QDate today = QDate::currentDate();
    int selYear = m_comboYear->currentData().toInt();
    int selMonth = m_comboMonth->currentData().toInt();
    if(selMonth == 0) selMonth = 1;

    int daysInMonth = QDate(selYear, selMonth, 1).daysInMonth();

    // 逻辑：如果是今年今月，从今天开始
    int startDay = (selYear == today.year() && selMonth == today.month()) ? today.day() : 1;

    for (int i = startDay; i <= daysInMonth; i++) {
        m_comboDay->addItem(QString("%1日").arg(i, 2, 10, QChar('0')), i);
    }
    m_comboDay->blockSignals(false);
}

void DateSelector::setDate(const QDate &date)
{
    int yIdx = m_comboYear->findData(date.year());
    if(yIdx != -1) m_comboYear->setCurrentIndex(yIdx);

    int mIdx = m_comboMonth->findData(date.month());
    if(mIdx != -1) m_comboMonth->setCurrentIndex(mIdx);

    int dIdx = m_comboDay->findData(date.day());
    if(dIdx != -1) m_comboDay->setCurrentIndex(dIdx);
}

QDate DateSelector::getDate() const
{
    return QDate(m_comboYear->currentData().toInt(),
                 m_comboMonth->currentData().toInt(),
                 m_comboDay->currentData().toInt());
}
