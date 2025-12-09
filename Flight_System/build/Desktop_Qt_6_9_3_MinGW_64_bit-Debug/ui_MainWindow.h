/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <usercenter.h>
#include "adbanner.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *leftContainer;
    QVBoxLayout *verticalLayout;
    QListWidget *menuList;
    QWidget *rightContainer;
    QStackedWidget *stackedWidget;
    QWidget *page0_First;
    AdBanner *bannerWidget;
    QWidget *destinationWidget;
    QLabel *label;
    QWidget *destinationWidget2;
    QWidget *destinationWidget3;
    QWidget *page2_Special;
    QWidget *page3_Find;
    QWidget *page4_Love;
    QWidget *page5_Order;
    QWidget *page7_About;
    UserCenter *page6_User;
    QWidget *page1_Flight;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(950, 629);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        leftContainer = new QWidget(MainWindow);
        leftContainer->setObjectName("leftContainer");
        leftContainer->setGeometry(QRect(6, 6, 187, 617));
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
        rightContainer = new QWidget(MainWindow);
        rightContainer->setObjectName("rightContainer");
        rightContainer->setGeometry(QRect(197, 6, 747, 617));
        sizePolicy.setHeightForWidth(rightContainer->sizePolicy().hasHeightForWidth());
        rightContainer->setSizePolicy(sizePolicy);
        stackedWidget = new QStackedWidget(rightContainer);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(-10, 10, 751, 621));
        page0_First = new QWidget();
        page0_First->setObjectName("page0_First");
        bannerWidget = new AdBanner(page0_First);
        bannerWidget->setObjectName("bannerWidget");
        bannerWidget->setGeometry(QRect(10, 0, 731, 161));
        sizePolicy.setHeightForWidth(bannerWidget->sizePolicy().hasHeightForWidth());
        bannerWidget->setSizePolicy(sizePolicy);
        destinationWidget = new QWidget(page0_First);
        destinationWidget->setObjectName("destinationWidget");
        destinationWidget->setGeometry(QRect(10, 270, 231, 301));
        sizePolicy.setHeightForWidth(destinationWidget->sizePolicy().hasHeightForWidth());
        destinationWidget->setSizePolicy(sizePolicy);
        label = new QLabel(page0_First);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 210, 101, 51));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\215\216\346\226\207\347\220\245\347\217\200")});
        font.setPointSize(25);
        font.setBold(false);
        label->setFont(font);
        destinationWidget2 = new QWidget(page0_First);
        destinationWidget2->setObjectName("destinationWidget2");
        destinationWidget2->setGeometry(QRect(260, 270, 231, 301));
        destinationWidget3 = new QWidget(page0_First);
        destinationWidget3->setObjectName("destinationWidget3");
        destinationWidget3->setGeometry(QRect(510, 270, 231, 301));
        stackedWidget->addWidget(page0_First);
        page2_Special = new QWidget();
        page2_Special->setObjectName("page2_Special");
        stackedWidget->addWidget(page2_Special);
        page3_Find = new QWidget();
        page3_Find->setObjectName("page3_Find");
        stackedWidget->addWidget(page3_Find);
        page4_Love = new QWidget();
        page4_Love->setObjectName("page4_Love");
        stackedWidget->addWidget(page4_Love);
        page5_Order = new QWidget();
        page5_Order->setObjectName("page5_Order");
        stackedWidget->addWidget(page5_Order);
        page7_About = new QWidget();
        page7_About->setObjectName("page7_About");
        stackedWidget->addWidget(page7_About);
        page6_User = new UserCenter();
        page6_User->setObjectName("page6_User");
        stackedWidget->addWidget(page6_User);
        page1_Flight = new QWidget();
        page1_Flight->setObjectName("page1_Flight");
        stackedWidget->addWidget(page1_Flight);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\345\223\210\345\244\253\345\205\213\350\210\252\347\217\255\347\263\273\347\273\237", nullptr));

        const bool __sortingEnabled = menuList->isSortingEnabled();
        menuList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = menuList->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MainWindow", "\351\246\226\351\241\265", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = menuList->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250\350\210\252\347\217\255", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = menuList->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("MainWindow", "\347\211\271\344\273\267\346\234\272\347\245\250", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = menuList->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("MainWindow", "\345\217\221\347\216\260", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = menuList->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("MainWindow", "\346\210\221\347\232\204\346\224\266\350\227\217", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = menuList->item(5);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("MainWindow", "\346\210\221\347\232\204\350\256\242\345\215\225", nullptr));
        QListWidgetItem *___qlistwidgetitem6 = menuList->item(6);
        ___qlistwidgetitem6->setText(QCoreApplication::translate("MainWindow", "\347\224\250\346\210\267\344\270\255\345\277\203", nullptr));
        QListWidgetItem *___qlistwidgetitem7 = menuList->item(7);
        ___qlistwidgetitem7->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216\346\210\221\344\273\254", nullptr));
        menuList->setSortingEnabled(__sortingEnabled);

        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p>\347\203\255\351\227\250\351\200\237\346\212\245</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
