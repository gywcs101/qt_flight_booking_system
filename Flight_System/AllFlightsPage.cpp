#include "AllFlightsPage.h"
#include "ui_AllFlightsPage.h"
#include "FlightCard.h"
#include "FlightData.h"
#include "UserSession.h"
#include "DateSelector.h"
#include "ODBC.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QMessageBox>
#include <QSqlDatabase>

AllFlightsPage::AllFlightsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllFlightsPage)
{
    ui->setupUi(this);

    // 1. 去除边框 (AllFlightsPage 看起来比较干净，通常都有这句)
    ui->scrollArea->setFrameShape(QFrame::NoFrame);

    // 2. 强制设置滚动条样式表 (这是一个类似于携程/现代App的细长滚动条样式)
    // 如果你刚才在 UI 文件里没找到样式，把这段代码复制进去，两个界面都会变好看
    QString scrollStyle = R"(
        QScrollArea {
            border: none;
            background-color: transparent;
        }
        QScrollBar:vertical {
            border: none;
            background: #F5F5F5;
            width: 8px; /* 滚动条宽度 */
            margin: 0px 0 0px 0;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #CCCCCC; /* 滑块颜色 */
            min-height: 20px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: #999999; /* 鼠标悬停变深 */
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px; /* 隐藏上下的箭头 */
        }
    )";

    // 应用样式表
    ui->scrollArea->setStyleSheet(scrollStyle);

    // 3. 确保滚动区域背景透明 (防止白色遮挡)
    ui->scrollArea->setAttribute(Qt::WA_TranslucentBackground);
    ui->scrollArea->viewport()->setAttribute(Qt::WA_TranslucentBackground); // 关键
    ui->scrollAreaWidgetContents->setAttribute(Qt::WA_TranslucentBackground);

    // 1. 布局优化
    if (this->layout()) this->layout()->setContentsMargins(0,0,0,0);
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *vbox = new QVBoxLayout(ui->scrollAreaWidgetContents);
        vbox->setSpacing(15);
        vbox->setContentsMargins(20, 20, 20, 20);
    }

    // 2. 初始化日期
    ui->dateSelector->setDate(QDate::currentDate());

    // 3. 连接查询按钮
    connect(ui->btnSearch, &QPushButton::clicked, this, &AllFlightsPage::loadFlightsData);

    // 4. 自动加载
    loadFlightsData();
}

AllFlightsPage::~AllFlightsPage()
{
    delete ui;
}

void AllFlightsPage::loadFlightsData()
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    // 清空界面
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    int uid = UserSession::instance().getUserId();

    // =================================================================
    // 【核心修改 1】 SQL 查询：排除已预订的航班
    // =================================================================
    // 逻辑：查找 flights 表，但排除掉 (NOT IN) 那些在 orders 表里 flight_id 和 user_id 都匹配的记录
    QString sql = QString("SELECT f.*, "
                          "(SELECT COUNT(*) FROM favorites WHERE user_id = %1 AND flight_id = f.flight_id) as is_fav "
                          "FROM flights f "
                          "WHERE f.flight_id NOT IN (SELECT flight_id FROM orders WHERE user_id = %1) "
                          ).arg(uid);

    // 拼接筛选条件
    if(!ui->lineEditDep->text().trimmed().isEmpty()) {
        sql += QString(" AND f.departure_city LIKE '%%1%'").arg(ui->lineEditDep->text().trimmed());
    }
    if(!ui->lineEditArr->text().trimmed().isEmpty()) {
        sql += QString(" AND f.arrival_city LIKE '%%1%'").arg(ui->lineEditArr->text().trimmed());
    }

    // 日期筛选
    QDate selDate = ui->dateSelector->getDate();
    sql += QString(" AND DATE(f.departure_time) = '%1'").arg(selDate.toString("yyyy-MM-dd"));

    // 排序
    sql += " ORDER BY f.departure_time ASC";

    // 执行查询
    QSqlQuery query = ODBC::query(sql);

    if (!query.isActive()) {
        qWarning() << "AllFlightsPage Query Failed:" << query.lastError().text();
        return;
    }

    int count = 0;
    int delayCounter = 0;

    while (query.next()) {
        count++;
        FlightData data;
        data.flightId = query.value("flight_id").toString();
        data.airline  = query.value("airline").toString();
        data.depCity  = query.value("departure_city").toString();
        data.arrCity  = query.value("arrival_city").toString();
        data.depTime  = query.value("departure_time").toDateTime();
        data.arrTime  = query.value("arrival_time").toDateTime();
        data.price    = query.value("price").toDouble();
        data.status   = query.value("status").toString();
        data.isFavorite = query.value("is_fav").toInt() > 0;

        FlightCard *card = new FlightCard(data, this);
        layout->addWidget(card);

        // --- 1. 收藏逻辑 ---
        connect(card, &FlightCard::favClicked, [=](const QString& fid, bool isFav){
            if (uid == -1) return;
            QString favSql;
            if (isFav) favSql = QString("INSERT INTO favorites (user_id, flight_id) VALUES (%1, '%2')").arg(uid).arg(fid);
            else favSql = QString("DELETE FROM favorites WHERE user_id = %1 AND flight_id = '%2'").arg(uid).arg(fid);
            ODBC::query(favSql);
        });

        // --- 2. 预订逻辑 (自动扣费 + 移除卡片) ---
        connect(card, &FlightCard::bookClicked, [=](QString fid) {
            if (uid == -1) {
                QMessageBox::warning(this, "提示", "请先登录后再预订机票！");
                return;
            }

            // A. 检查余额
            QString balanceSql = QString("SELECT balance FROM users WHERE id = %1").arg(uid);
            QSqlQuery balanceQ = ODBC::query(balanceSql);
            if (balanceQ.next()) {
                double currentBalance = balanceQ.value("balance").toDouble();

                if (currentBalance < data.price) {
                    QMessageBox::warning(this, "余额不足",
                                         QString("您的余额 (¥%1) 不足支付 (¥%2)，请充值！").arg(currentBalance).arg(data.price));
                    return;
                }

                // B. 开启事务
                QSqlDatabase::database().transaction();
                bool success = true;

                // 扣款
                QString deductSql = QString("UPDATE users SET balance = balance - %1 WHERE id = %2")
                                        .arg(data.price).arg(uid);
                if (ODBC::query(deductSql).lastError().isValid()) success = false;

                // 生成订单
                QString orderSql = QString("INSERT INTO orders (user_id, flight_id, price, status, create_time) "
                                           "VALUES (%1, '%2', %3, '已支付', NOW())")
                                       .arg(uid).arg(fid).arg(data.price);
                if (ODBC::query(orderSql).lastError().isValid()) success = false;

                // C. 提交或回滚
                if (success) {
                    QSqlDatabase::database().commit();

                    QMessageBox::information(this, "预订成功",
                                             QString("预订成功！\n已自动扣除 ¥%1。\n您可以在“我的订单”中查看详情。").arg(data.price));

                    // 【核心修改 2】 预订成功后，立即从界面移除该卡片
                    layout->removeWidget(card); // 1. 从布局移除
                    card->deleteLater();        // 2. 销毁对象
                } else {
                    QSqlDatabase::database().rollback();
                    QMessageBox::critical(this, "预订失败", "系统繁忙，交易已取消，请重试。");
                }
            }
        });
        card->startEntryAnimation(delayCounter * 80); // 每个间隔 80ms
        delayCounter++;
    }

    // 暂无数据提示
    if(count == 0){
        QLabel* tipLabel = new QLabel("未找到符合条件的航班\n(或者该日期的航班您已全部预订)", this);
        tipLabel->setAlignment(Qt::AlignCenter);
        tipLabel->setStyleSheet("color: #999; font-size: 16px;");
        layout->addWidget(tipLabel);
    }

    layout->addStretch();
}
