#include "FavoritesPage.h"
#include "ui_FavoritesPage.h"
#include "FlightCard.h"
#include "UserSession.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

FavoritesPage::FavoritesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FavoritesPage)
{
    ui->setupUi(this);

    // =========================================================
    // 1. 深度美化样式表 (QSS)
    // =========================================================
    QString qss = R"(
        /* 全局背景：淡灰 */
        QWidget#FavoritesPage {
            background-color: #F5F7FA;
        }

        /* 标题样式：大号、加粗、深灰 */
        QLabel#lblTitle {
            font-family: "Microsoft YaHei";
            font-size: 28px;
            font-weight: bold;
            color: #333333;
            padding: 20px 0 10px 20px; /* 上 右 下 左 */
        }

        /* 滚动区域：透明背景，无边框 */
        QScrollArea {
            background-color: transparent;
            border: none;
        }
        QWidget#scrollAreaWidgetContents {
            background-color: transparent; /* 内容区也透明 */
        }

        /* 美化垂直滚动条 - 极简风格 */
        QScrollBar:vertical {
            border: none;
            background: #F5F7FA;
            width: 8px; /* 细一点 */
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #C0C4CC; /* 浅灰滑块 */
            border-radius: 4px;
            min-height: 40px;
        }
        QScrollBar::handle:vertical:hover {
            background: #909399; /* 悬停变深 */
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px; /* 隐藏上下箭头 */
        }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
        }
    )";
    this->setStyleSheet(qss);

    // =========================================================
    // 2. 布局初始化
    // =========================================================
    // 确保内部容器有布局
    if (!ui->scrollAreaWidgetContents->layout()) {
        QVBoxLayout *vbox = new QVBoxLayout(ui->scrollAreaWidgetContents);
        vbox->setSpacing(15); // 卡片之间的间距 15px
        // 内容区的边距：左20, 上0, 右20, 下20 (给滚动条留点位置)
        vbox->setContentsMargins(20, 0, 20, 20);
        // 顶部对齐，防止卡片数量少时分散
        vbox->setAlignment(Qt::AlignTop);
    }
}

FavoritesPage::~FavoritesPage()
{
    delete ui;
}

void FavoritesPage::loadFavoriteFlights()
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    // 1. 清空旧列表 (卡片 + 空状态提示)
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) delete child->widget();
        delete child;
    }

    // 2. 获取用户ID
    int uid = UserSession::instance().getUserId();
    if (uid == -1) {
        showEmptyState(layout, "请先登录查看收藏");
        return;
    }

    // 3. 联表查询
    QString sql = "SELECT f.* "
                  "FROM flights f "
                  "INNER JOIN favorites fav ON f.flight_id = fav.flight_id "
                  "WHERE fav.user_id = :uid "
                  "ORDER BY fav.create_time DESC";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":uid", uid);

    if (!query.exec()) {
        qDebug() << "Fav load error:" << query.lastError().text();
        return;
    }

    int count = 0;
    while (query.next()) {
        count++;
        FlightData data;
        // 填充数据...
        data.flightId = query.value("flight_id").toString();
        data.airline  = query.value("airline").toString();
        data.depCity  = query.value("departure_city").toString();
        data.arrCity  = query.value("arrival_city").toString();
        data.depTime  = query.value("departure_time").toDateTime();
        data.arrTime  = query.value("arrival_time").toDateTime();
        data.price    = query.value("price").toDouble();
        data.status   = query.value("status").toString();
        data.isFavorite = true;

        FlightCard *card = new FlightCard(data);
        layout->addWidget(card);

        // 处理“取消收藏” -> 动效移除
        connect(card, &FlightCard::favClicked, [=](QString fid, bool isFavNow){
            if (!isFavNow) {
                // 1. 删库
                QSqlQuery delQ;
                delQ.prepare("DELETE FROM favorites WHERE user_id = :uid AND flight_id = :fid");
                delQ.bindValue(":uid", uid);
                delQ.bindValue(":fid", fid);
                delQ.exec();

                // 2. 删界面 (稍微延时一点删除，防止崩，或者直接用 deleteLater)
                layout->removeWidget(card);
                card->deleteLater();

                // 3. 【优化】如果删完之后没有卡片了，显示空状态
                // 这是一个稍微高级点的判断：如果当前 layout 里只剩这一个控件了
                if(layout->count() <= 1) { // 1是因为card还没彻底delete
                    showEmptyState(layout, "您还没有收藏任何航班\n快去添加一些吧！");
                }
            }
        });

        // 处理预订点击
        connect(card, &FlightCard::bookClicked, [=](QString id){
            qDebug() << "收藏页点击预订:" << id;
        });
    }

    // 4. 如果没有数据，显示美化的空状态
    if (count == 0) {
        showEmptyState(layout, "您还没有收藏任何航班\n快去添加一些吧！");
    }

    // 这一步很重要：如果是非空状态，要在底部加一个弹簧，把卡片顶上去
    // 但是如果是空状态，我们在 showEmptyState 里已经处理了居中，所以这里判断一下
    if (count > 0) {
        layout->addStretch();
    }
}

// 【辅助函数】生成一个漂亮的空状态界面
void FavoritesPage::showEmptyState(QVBoxLayout *layout, QString text)
{
    // 创建一个容器 Widget
    QWidget *emptyWidget = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout(emptyWidget);

    // 1. 图标 (可以用 Emoji，也可以用图片)
    QLabel *icon = new QLabel("📂");
    // 如果你有图片，用 icon->setPixmap(QPixmap(":/icons/empty.png"));
    icon->setStyleSheet("font-size: 60px; color: #DCDFE6;");
    icon->setAlignment(Qt::AlignCenter);

    // 2. 文字提示
    QLabel *msg = new QLabel(text);
    msg->setStyleSheet("font-size: 16px; color: #909399; font-weight: bold;");
    msg->setAlignment(Qt::AlignCenter);

    vbox->addStretch(); // 上弹簧
    vbox->addWidget(icon);
    vbox->addWidget(msg);
    vbox->addStretch(); // 下弹簧

    layout->addWidget(emptyWidget);
}
