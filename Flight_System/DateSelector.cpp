#include "DateSelector.h"
#include <QHBoxLayout>
#include <QListView> // 必须包含，用于美化下拉列表视图
#include <QDebug>

DateSelector::DateSelector(QWidget *parent) : QWidget(parent)
{
    // 1. 初始化界面
    setupUi();

    // 2. 初始化为今天
    setDate(QDate::currentDate());

    // 3. 信号连接
    // 当月份改变时 -> 重新计算该月有多少天
    connect(m_comboMonth, &QComboBox::currentIndexChanged, this, &DateSelector::updateDays);

    // 当天数改变时 -> 发出信号通知外部
    connect(m_comboDay, &QComboBox::currentIndexChanged, [this]() {
        // 过滤掉 updateDays 过程中清空列表时产生的无效信号
        if (m_comboDay->count() > 0) {
            emit dateChanged(getDate());
        }
    });
}

void DateSelector::setupUi()
{
    // === 布局设置 ===
    // 使用 this 作为父对象，确保控件显示在 DateSelector 内部
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0); // 去掉边距，让控件贴边
    layout->setSpacing(8);                  // 控件之间的间隙

    // === 1. 年份 Label ===
    // 获取当前年份
    int currentYear = QDate::currentDate().year();
    m_lblYear = new QLabel(QString::number(currentYear) + "年", this);
    m_lblYear->setStyleSheet("color: #555555; font-weight: bold; font-size: 14px; font-family: 'Microsoft YaHei';");
    // 垂直居中
    m_lblYear->setAlignment(Qt::AlignVCenter);

    // === 2. 样式表 (QSS) ===
    // 这种样式模仿了现代 Web 的下拉框风格
    QString comboStyle = R"(
        QComboBox {
            border: 1px solid #DCDFE6;
            border-radius: 4px;
            padding: 4px 10px;
            min-width: 60px;  /* 最小宽度，防止太窄 */
            background-color: white;
            color: #333333;
            font-size: 14px;
            font-family: 'Microsoft YaHei';
        }
        QComboBox:hover {
            border: 1px solid #409EFF; /* 悬停变蓝 */
            background-color: #F2F6FC;
        }
        QComboBox:on { /* 下拉菜单打开时 */
            border: 1px solid #409EFF;
        }
        QComboBox::drop-down {
            width: 20px;
            border: none;
            subcontrol-origin: padding;
            subcontrol-position: top right;
        }
        QComboBox::down-arrow {
            /* 这里可以放一个箭头图片，如果想极简可以设为 none */
            image: none;
            border: none;
        }
        /* 下拉列表项的样式 */
        QComboBox QAbstractItemView {
            border: 1px solid #DCDFE6;
            selection-background-color: #409EFF;
            selection-color: white;
            background-color: white;
            outline: none;
            padding: 2px;
        }
    )";

    // === 3. 月份选择 ===
    m_comboMonth = new QComboBox(this);
    m_comboMonth->setView(new QListView()); // 关键：设置 View 才能让 QSS 对下拉项高度生效
    m_comboMonth->setStyleSheet(comboStyle);
    // 填充 1-12 月
    for (int i = 1; i <= 12; i++) {
        m_comboMonth->addItem(QString("%1月").arg(i, 2, 10, QChar('0')), i); // 显示"01月", 数据存 int i
    }

    // === 4. 日期选择 ===
    m_comboDay = new QComboBox(this);
    m_comboDay->setView(new QListView());
    m_comboDay->setStyleSheet(comboStyle);
    // 天数稍后由 updateDays 填充

    // === 5. 添加到布局 ===
    layout->addWidget(m_lblYear);
    layout->addWidget(m_comboMonth);
    layout->addWidget(m_comboDay);

    // === 6. 强制设置控件大小 ===
    // 这一步非常重要，防止 Layout 把它压缩没了
    this->setMinimumSize(200, 40);
}

void DateSelector::updateDays()
{
    // 1. 暂停信号，防止在添加 Item 时频繁触发 dateChanged
    m_comboDay->blockSignals(true);

    // 2. 记住当前选的是几号 (如果是第一次加载，默认设为今天)
    int currentDay = m_comboDay->currentData().toInt();
    if (currentDay == 0) currentDay = QDate::currentDate().day();

    // 3. 获取当前选的年和月
    int year = QDate::currentDate().year();
    // currentData() 返回我们在 addItem 时存入的 int i
    int month = m_comboMonth->currentData().toInt();
    if (month == 0) month = 1; // 防御性编程

    // 4. 计算该月有多少天 (Qt 自带函数，自动处理闰年)
    int days = QDate(year, month, 1).daysInMonth();

    // 5. 清空并重新填充
    m_comboDay->clear();
    for (int i = 1; i <= days; i++) {
        m_comboDay->addItem(QString("%1日").arg(i, 2, 10, QChar('0')), i);
    }

    // 6. 尝试恢复之前的选择
    // 比如你之前选了31号，切到2月(只有28天)，则自动选28号
    if (currentDay > days) {
        currentDay = days;
    }

    // 找到对应的索引并选中
    int index = m_comboDay->findData(currentDay);
    if (index != -1) {
        m_comboDay->setCurrentIndex(index);
    }

    // 7. 恢复信号
    m_comboDay->blockSignals(false);

    // 8. 手动通知一次外部：日期变了（因为可能从31号变成了28号）
    emit dateChanged(getDate());
}

QDate DateSelector::getDate() const
{
    int year = QDate::currentDate().year();
    int month = m_comboMonth->currentData().toInt();
    int day = m_comboDay->currentData().toInt();

    // 防御性编程：如果未初始化完成
    if (month == 0) month = 1;
    if (day == 0) day = 1;

    return QDate(year, month, day);
}

void DateSelector::setDate(const QDate &date)
{
    if (!date.isValid()) return;

    // 1. 设置月份 (Index 从 0 开始，而月份是 1-12)
    int monthIndex = m_comboMonth->findData(date.month());
    if (monthIndex != -1) {
        // 这里会触发 updateDays，从而自动填好天数
        m_comboMonth->setCurrentIndex(monthIndex);
    }

    // 2. 设置天数
    // 由于 setCurrentIndex 触发了 updateDays，列表已经刷新了，现在可以直接选天数
    int dayIndex = m_comboDay->findData(date.day());
    if (dayIndex != -1) {
        m_comboDay->setCurrentIndex(dayIndex);
    }
}
