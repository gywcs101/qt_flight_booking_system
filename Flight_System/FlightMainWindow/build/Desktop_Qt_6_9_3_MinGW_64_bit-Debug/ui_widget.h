/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *leftContainer;
    QVBoxLayout *verticalLayout;
    QListWidget *menuList;
    QWidget *rightContainer;
    QStackedWidget *mainStack;
    QWidget *page;
    QWidget *page_8;
    QWidget *page_7;
    QWidget *page_6;
    QWidget *page_5;
    QWidget *page_4;
    QWidget *page_3;
    QWidget *page_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(950, 629);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Widget->sizePolicy().hasHeightForWidth());
        Widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(Widget);
        horizontalLayout->setObjectName("horizontalLayout");
        leftContainer = new QWidget(Widget);
        leftContainer->setObjectName("leftContainer");
        sizePolicy.setHeightForWidth(leftContainer->sizePolicy().hasHeightForWidth());
        leftContainer->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(leftContainer);
        verticalLayout->setObjectName("verticalLayout");
        menuList = new QListWidget(leftContainer);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/main.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(menuList);
        __qlistwidgetitem->setIcon(icon);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/flight3.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(menuList);
        __qlistwidgetitem1->setIcon(icon1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/account.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(menuList);
        __qlistwidgetitem2->setIcon(icon2);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/find.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(menuList);
        __qlistwidgetitem3->setIcon(icon3);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/like.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(menuList);
        __qlistwidgetitem4->setIcon(icon4);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/dingdan.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem5 = new QListWidgetItem(menuList);
        __qlistwidgetitem5->setIcon(icon5);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/user.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem6 = new QListWidgetItem(menuList);
        __qlistwidgetitem6->setIcon(icon6);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/about.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem7 = new QListWidgetItem(menuList);
        __qlistwidgetitem7->setIcon(icon7);
        menuList->setObjectName("menuList");
        menuList->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        menuList->setFrameShape(QFrame::Shape::NoFrame);
        menuList->setIconSize(QSize(244, 24));
        menuList->setSpacing(8);

        verticalLayout->addWidget(menuList);

        verticalLayout->setStretch(0, 8);

        horizontalLayout->addWidget(leftContainer);

        rightContainer = new QWidget(Widget);
        rightContainer->setObjectName("rightContainer");
        sizePolicy.setHeightForWidth(rightContainer->sizePolicy().hasHeightForWidth());
        rightContainer->setSizePolicy(sizePolicy);
        mainStack = new QStackedWidget(rightContainer);
        mainStack->setObjectName("mainStack");
        mainStack->setGeometry(QRect(0, 0, 751, 621));
        sizePolicy.setHeightForWidth(mainStack->sizePolicy().hasHeightForWidth());
        mainStack->setSizePolicy(sizePolicy);
        page = new QWidget();
        page->setObjectName("page");
        mainStack->addWidget(page);
        page_8 = new QWidget();
        page_8->setObjectName("page_8");
        mainStack->addWidget(page_8);
        page_7 = new QWidget();
        page_7->setObjectName("page_7");
        mainStack->addWidget(page_7);
        page_6 = new QWidget();
        page_6->setObjectName("page_6");
        mainStack->addWidget(page_6);
        page_5 = new QWidget();
        page_5->setObjectName("page_5");
        mainStack->addWidget(page_5);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        mainStack->addWidget(page_4);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        mainStack->addWidget(page_3);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        mainStack->addWidget(page_2);

        horizontalLayout->addWidget(rightContainer);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 4);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "\345\223\210\345\244\253\345\205\213\350\210\252\347\217\255\347\263\273\347\273\237", nullptr));

        const bool __sortingEnabled = menuList->isSortingEnabled();
        menuList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = menuList->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("Widget", "\351\246\226\351\241\265", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = menuList->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("Widget", "\345\205\250\351\203\250\350\210\252\347\217\255", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = menuList->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("Widget", "\347\211\271\344\273\267\346\234\272\347\245\250", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = menuList->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("Widget", "\345\217\221\347\216\260", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = menuList->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("Widget", "\346\210\221\347\232\204\346\224\266\350\227\217", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = menuList->item(5);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("Widget", "\346\210\221\347\232\204\350\256\242\345\215\225", nullptr));
        QListWidgetItem *___qlistwidgetitem6 = menuList->item(6);
        ___qlistwidgetitem6->setText(QCoreApplication::translate("Widget", "\347\224\250\346\210\267\344\270\255\345\277\203", nullptr));
        QListWidgetItem *___qlistwidgetitem7 = menuList->item(7);
        ___qlistwidgetitem7->setText(QCoreApplication::translate("Widget", "\345\205\263\344\272\216\346\210\221\344\273\254", nullptr));
        menuList->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
