#include "DiscoveryPage.h"
#include "ui_DiscoveryPage.h"
#include "PostCard.h"
#include "DetailDialog.h"
#include "ODBC.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DiscoveryPage::DiscoveryPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiscoveryPage)
{
    ui->setupUi(this);
    initUi();
    loadData();
}

DiscoveryPage::~DiscoveryPage()
{
    delete ui;
}

void DiscoveryPage::initUi()
{
    if (!ui->scrollAreaWidgetContents->layout()) {
        gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    } else {
        gridLayout = qobject_cast<QGridLayout*>(ui->scrollAreaWidgetContents->layout());
    }

    gridLayout->setContentsMargins(20, 20, 20, 20);
    gridLayout->setSpacing(20);

    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 1);

    // 1. 设置滚动区无边框，背景透明
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    ui->scrollArea->setAttribute(Qt::WA_TranslucentBackground);
    ui->scrollArea->viewport()->setAttribute(Qt::WA_TranslucentBackground);

    // 2. 现代风格滚动条样式表 (所有界面统一)
    QString commonStyle = R"(
        QScrollArea { border: none; background: transparent; }
        QScrollBar:vertical {
            border: none;
            background: #F5F5F5;      /* 浅灰轨道 */
            width: 8px;               /* 宽度8像素 */
            margin: 0px 0 0px 0;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #C0C0C0;      /* 银灰滑块 */
            min-height: 20px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: #909090;      /* 悬停变深 */
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;              /* 隐藏箭头 */
        }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
        }
    )";

    // 3. 应用样式
    ui->scrollArea->setStyleSheet(commonStyle);
}

void DiscoveryPage::loadData()
{
    QLayoutItem *child;
    while ((child = gridLayout->takeAt(0)) != nullptr) {
        if(child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    QSqlQuery query = ODBC::query("SELECT * FROM discovery_posts ORDER BY id DESC");

    if(!query.isActive()){
        qWarning() << "DiscoveryPage: Failed to execute query." << query.lastError();
        return;
    }

    int row = 0;
    int col = 0;
    const int maxCols = 3;

    while (query.next()) {
        PostData data;
        data.id = query.value("id").toInt();

        data.title = QString::fromUtf8(query.value("title").toByteArray());
        data.content = QString::fromUtf8(query.value("content").toByteArray());
        data.authorName = QString::fromUtf8(query.value("author_name").toByteArray());
        data.imagePath = QString::fromUtf8(query.value("image_path").toByteArray());
        data.avatarPath = QString::fromUtf8(query.value("avatar_path").toByteArray());

        PostCard *card = new PostCard(data);
        connect(card, &PostCard::cardClicked, this, &DiscoveryPage::onCardClicked);
        gridLayout->addWidget(card, row, col);

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }

    gridLayout->setRowStretch(row + 1, 1);
}

void DiscoveryPage::refreshPosts()
{
    loadData();
}


void DiscoveryPage::onCardClicked(const PostData &data)
{
    DetailDialog *dialog = new DetailDialog(data, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}
