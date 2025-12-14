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
    // 1. 获取布局指针
    // 注意：这里必须强转为 QVBoxLayout 才能使用 addStretch
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    // 2. 清空当前界面所有内容 (旧卡片 + 旧的弹簧 + 旧的空状态提示)
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if(child->widget()) {
            child->widget()->deleteLater(); // 安全删除控件
        }
        delete child; // 删除布局项
    }

    // 3. 获取当前登录用户ID
    int uid = UserSession::instance().getUserId();

    // 【调试信息】帮助你排查为什么不显示
    qDebug() << "正在加载收藏，当前用户ID:" << uid;

    if (uid == -1) {
        showEmptyState(layout, "请先登录查看收藏");
        layout->addStretch(); // 顶上去
        return;
    }

    // 4. 准备 SQL：联表查询 (INNER JOIN)
    // 逻辑：只查 flights 表中那些在 favorites 表里有记录的航班
    QString sql = "SELECT f.* "
                  "FROM flights f "
                  "INNER JOIN favorites fav ON f.flight_id = fav.flight_id "
                  "WHERE fav.user_id = :uid "
                  "ORDER BY fav.create_time DESC"; // 按收藏时间倒序排列

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":uid", uid);

    if (!query.exec()) {
        qDebug() << "收藏列表查询失败:" << query.lastError().text();
        showEmptyState(layout, "数据加载失败，请检查数据库");
        return;
    }

    // 5. 遍历结果生成卡片
    int count = 0;
    while (query.next()) {
        count++;
        FlightData data;
        // 填充数据
        data.flightId = query.value("flight_id").toString();
        data.airline  = query.value("airline").toString();
        data.depCity  = query.value("departure_city").toString();
        data.arrCity  = query.value("arrival_city").toString();
        data.depTime  = query.value("departure_time").toDateTime();
        data.arrTime  = query.value("arrival_time").toDateTime();
        data.price    = query.value("price").toDouble();
        data.status   = query.value("status").toString();

        // 【关键】在收藏页显示的，肯定是已收藏状态
        data.isFavorite = true;

        FlightCard *card = new FlightCard(data);
        layout->addWidget(card);

        // 6. 处理“取消收藏”逻辑 -> 动态移除
        connect(card, &FlightCard::favClicked, [=](QString fid, bool isFavNow){
            // 如果在收藏页点击了，肯定是变成 false (取消收藏)
            if (!isFavNow) {
                // A. 删数据库
                QSqlQuery delQ;
                delQ.prepare("DELETE FROM favorites WHERE user_id = :uid AND flight_id = :fid");
                delQ.bindValue(":uid", uid);
                delQ.bindValue(":fid", fid);
                if(delQ.exec()) {
                    qDebug() << "已移除收藏:" << fid;
                }

                // B. 删界面 (视觉上立即消失)
                layout->removeWidget(card);
                card->deleteLater();

                // C. 【体验优化】如果删完后一张卡片都没了，显示空状态
                // layout->count() 包含弹簧，所以如果只剩1个(弹簧)或0个，就说明空了
                // 为保险起见，我们遍历一下看看还有没有 FlightCard
                bool hasCard = false;
                for(int i=0; i<layout->count(); ++i) {
                    // 如果布局里还有 FlightCard 类型的控件
                    if(qobject_cast<FlightCard*>(layout->itemAt(i)->widget())) {
                        hasCard = true;
                        break;
                    }
                }

                if(!hasCard) {
                    // 移除底部的弹簧(如果存在)
                    QLayoutItem *item;
                    while ((item = layout->takeAt(0)) != nullptr) {
                        if(item->widget()) item->widget()->deleteLater();
                        delete item;
                    }
                    showEmptyState(layout, "您还没有收藏任何航班\n快去添加一些吧！");
                    layout->addStretch();
                }
            }
        });

        // 处理预订点击
        connect(card, &FlightCard::bookClicked, [=](QString id){
            qDebug() << "收藏页点击预订:" << id;
        });
    }

    // 7. 处理结果为空的情况
    if (count == 0) {
        showEmptyState(layout, "您还没有收藏任何航班\n快去添加一些吧！");
    }

    // 8. 必须加弹簧，把卡片顶到最上面
    layout->addStretch();
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
