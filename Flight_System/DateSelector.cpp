#include "DateSelector.h"
#include <QHBoxLayout>
#include <QListView> // 用于美化下拉列表视图
#include <QDebug>

DateSelector::DateSelector(QWidget *parent) : QWidget(parent)
{
    setupUi();

    // 默认初始化为今天
    setDate(QDate::currentDate());

    // 连接信号
    // 月份变了 -> 更新天数 -> 发送信号
    connect(m_comboMonth, &QComboBox::currentIndexChanged, this, &DateSelector::updateDays);

    // 天数变了 -> 发送信号
    connect(m_comboDay, &QComboBox::currentIndexChanged, [this](){
        emit dateChanged(getDate());
    });
}

void DateSelector::setupUi()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8); // 控件间距

    // 1. 年份 Label (固定为今年，颜色深灰)
    int currentYear = QDate::currentDate().year();
    m_lblYear = new QLabel(QString::number(currentYear) + "年");
    m_lblYear->setStyleSheet("color: #555; font-weight: bold; font-size: 14px;");

    // 2. 样式表 (QSS) - 扁平化设计
    QString comboStyle = R"(
        QComboBox {
            border: 1px solid #E0E0E0;
            border-radius: 6px;
            padding: 4px 10px;
            min-width: 50px;
            background-color: white;
            color: #333;
            font-size: 14px;
            font-weight: bold;
        }
        QComboBox:hover {
            border: 1px solid #0078D7;
            background-color: #F8FBFF;
        }
        QComboBox::drop-down {
            width: 20px;
            border: none;
            subcontrol-origin: padding;
            subcontrol-position: top right;
        }
        QComboBox::down-arrow {
            image: none; /* 隐藏默认箭头，保持极简 */
        }
        QComboBox QAbstractItemView {
            border: 1px solid #DDD;
            selection-background-color: #0078D7;
            selection-color: white;
            outline: none;
        }
    )";

    // 3. 月份选择
    m_comboMonth = new QComboBox;
    m_comboMonth->setView(new QListView()); // 启用样式
    m_comboMonth->setStyleSheet(comboStyle);
    for(int i=1; i<=12; i++) {
        m_comboMonth->addItem(QString("%1月").arg(i, 2, 10, QChar('0')), i);
    }

    // 4. 日期选择
    m_comboDay = new QComboBox;
    m_comboDay->setView(new QListView());
    m_comboDay->setStyleSheet(comboStyle);
    // 天数稍后由 updateDays 填充

    layout->addWidget(m_lblYear);
    layout->addWidget(m_comboMonth);
    layout->addWidget(m_comboDay);

    // 限制整体高度
    this->setFixedHeight(35);
}

void DateSelector::updateDays()
{
    // 1. 记住当前选的是几号
    int currentDay = m_comboDay->currentData().toInt();

    // 2. 获取当前选的年和月
    int year = QDate::currentDate().year();
    int month = m_comboMonth->currentData().toInt();

    // 3. 计算该月有多少天
    int days = QDate(year, month, 1).daysInMonth();

    // 4. 重新填充天数下拉框
    m_comboDay->blockSignals(true); // 暂停信号防止闪烁
    m_comboDay->clear();
    for(int i=1; i<=days; i++) {
        m_comboDay->addItem(QString("%1日").arg(i, 2, 10, QChar('0')), i);
    }
    m_comboDay->blockSignals(false);

    // 5. 尝试恢复之前的选择 (如果没有越界)
    int index = m_comboDay->findData(currentDay);
    if(index != -1) {
        m_comboDay->setCurrentIndex(index);
    } else {
        m_comboDay->setCurrentIndex(0); // 否则选1号
    }

    // 手动触发一次信号
    emit dateChanged(getDate());
}

QDate DateSelector::getDate() const
{
    int year = QDate::currentDate().year();
    int month = m_comboMonth->currentData().toInt();
    int day = m_comboDay->currentData().toInt();

    // 如果还没初始化完，可能读到0，做个保护
    if(day == 0) day = 1;

    return QDate(year, month, day);
}

void DateSelector::setDate(const QDate &date)
{
    // 设置月份 (索引从0开始)
    int monthIndex = date.month() - 1;
    if(monthIndex >= 0 && monthIndex < 12) {
        m_comboMonth->setCurrentIndex(monthIndex);
    }

    // 强制更新天数列表
    updateDays();

    // 设置天数
    int dayIndex = m_comboDay->findData(date.day());
    if(dayIndex != -1) {
        m_comboDay->setCurrentIndex(dayIndex);
    }
}
