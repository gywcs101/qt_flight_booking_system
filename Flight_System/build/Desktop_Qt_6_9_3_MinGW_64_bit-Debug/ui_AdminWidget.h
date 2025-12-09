/********************************************************************************
** Form generated from reading UI file 'AdminWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINWIDGET_H
#define UI_ADMINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QListWidget *listMenu;
    QWidget *widget_2;
    QStackedWidget *stackedContent;
    QWidget *page;
    QTableWidget *tableUser;
    QPushButton *btn_del;
    QPushButton *btn_add;
    QPushButton *btn_modify;
    QLabel *lbl_username;
    QLabel *lbl_password;
    QLabel *lbl_phone;
    QLineEdit *lineEdit_username;
    QLineEdit *lineEdit_phone;
    QLineEdit *lineEdit_password;
    QLabel *label;
    QLabel *label_2;
    QWidget *page_3;
    QLabel *label_4;
    QLabel *lbl_flightID;
    QLabel *lbl_departure;
    QLabel *lbl_destination;
    QLabel *lbl_capicity;
    QLabel *lbl_dptTime;
    QLabel *lbl_arrTime;
    QLabel *lbl_price;
    QLineEdit *lineEdit_flightID;
    QLineEdit *lineEdit_departure;
    QLineEdit *lineEdit_destination;
    QLineEdit *lineEdit_capacity;
    QLineEdit *lineEdit_dptTime;
    QLineEdit *lineEdit_arrTime;
    QLineEdit *lineEdit_price;
    QLabel *label_5;
    QTableWidget *tableFlight;
    QPushButton *btn_add_3;
    QPushButton *btn_del_3;
    QPushButton *btn_modify_3;
    QWidget *page_2;
    QLabel *label_3;
    QLabel *lbl_managername;
    QLabel *lbl_password_2;
    QLabel *lbl_phone_2;
    QLineEdit *lineEdit_managername;
    QLineEdit *lineEdit_password_2;
    QLineEdit *lineEdit_phone_2;
    QLabel *label_7;
    QTableWidget *tableManager;
    QPushButton *btn_add_2;
    QPushButton *btn_del_2;
    QPushButton *btn_modify_2;

    void setupUi(QWidget *AdminWidget)
    {
        if (AdminWidget->objectName().isEmpty())
            AdminWidget->setObjectName("AdminWidget");
        AdminWidget->resize(950, 629);
        horizontalLayout = new QHBoxLayout(AdminWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        widget = new QWidget(AdminWidget);
        widget->setObjectName("widget");
        listMenu = new QListWidget(widget);
        new QListWidgetItem(listMenu);
        new QListWidgetItem(listMenu);
        new QListWidgetItem(listMenu);
        listMenu->setObjectName("listMenu");
        listMenu->setGeometry(QRect(0, 0, 191, 621));
        listMenu->setLineWidth(1);
        listMenu->setIconSize(QSize(24, 24));
        listMenu->setSpacing(8);

        horizontalLayout->addWidget(widget);

        widget_2 = new QWidget(AdminWidget);
        widget_2->setObjectName("widget_2");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        stackedContent = new QStackedWidget(widget_2);
        stackedContent->setObjectName("stackedContent");
        stackedContent->setGeometry(QRect(0, -10, 741, 611));
        page = new QWidget();
        page->setObjectName("page");
        tableUser = new QTableWidget(page);
        tableUser->setObjectName("tableUser");
        tableUser->setGeometry(QRect(20, 140, 721, 371));
        btn_del = new QPushButton(page);
        btn_del->setObjectName("btn_del");
        btn_del->setGeometry(QRect(360, 550, 91, 31));
        QFont font;
        font.setPointSize(12);
        btn_del->setFont(font);
        btn_add = new QPushButton(page);
        btn_add->setObjectName("btn_add");
        btn_add->setGeometry(QRect(120, 550, 91, 31));
        btn_add->setFont(font);
        btn_modify = new QPushButton(page);
        btn_modify->setObjectName("btn_modify");
        btn_modify->setGeometry(QRect(590, 550, 91, 31));
        btn_modify->setFont(font);
        lbl_username = new QLabel(page);
        lbl_username->setObjectName("lbl_username");
        lbl_username->setGeometry(QRect(60, 70, 51, 21));
        lbl_password = new QLabel(page);
        lbl_password->setObjectName("lbl_password");
        lbl_password->setGeometry(QRect(310, 70, 61, 20));
        QFont font1;
        font1.setPointSize(11);
        lbl_password->setFont(font1);
        lbl_phone = new QLabel(page);
        lbl_phone->setObjectName("lbl_phone");
        lbl_phone->setGeometry(QRect(520, 70, 61, 16));
        lineEdit_username = new QLineEdit(page);
        lineEdit_username->setObjectName("lineEdit_username");
        lineEdit_username->setGeometry(QRect(110, 60, 121, 31));
        lineEdit_phone = new QLineEdit(page);
        lineEdit_phone->setObjectName("lineEdit_phone");
        lineEdit_phone->setGeometry(QRect(580, 60, 121, 31));
        lineEdit_password = new QLineEdit(page);
        lineEdit_password->setObjectName("lineEdit_password");
        lineEdit_password->setGeometry(QRect(350, 60, 121, 31));
        label = new QLabel(page);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 110, 721, 20));
        label_2 = new QLabel(page);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 20, 721, 16));
        stackedContent->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        label_4 = new QLabel(page_3);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 20, 731, 16));
        lbl_flightID = new QLabel(page_3);
        lbl_flightID->setObjectName("lbl_flightID");
        lbl_flightID->setGeometry(QRect(20, 60, 51, 21));
        lbl_flightID->setFont(font1);
        lbl_departure = new QLabel(page_3);
        lbl_departure->setObjectName("lbl_departure");
        lbl_departure->setGeometry(QRect(200, 60, 51, 21));
        lbl_departure->setFont(font1);
        lbl_destination = new QLabel(page_3);
        lbl_destination->setObjectName("lbl_destination");
        lbl_destination->setGeometry(QRect(380, 60, 51, 21));
        lbl_destination->setFont(font1);
        lbl_capicity = new QLabel(page_3);
        lbl_capicity->setObjectName("lbl_capicity");
        lbl_capicity->setGeometry(QRect(560, 60, 51, 21));
        lbl_capicity->setFont(font1);
        lbl_dptTime = new QLabel(page_3);
        lbl_dptTime->setObjectName("lbl_dptTime");
        lbl_dptTime->setGeometry(QRect(10, 110, 61, 21));
        lbl_dptTime->setFont(font1);
        lbl_arrTime = new QLabel(page_3);
        lbl_arrTime->setObjectName("lbl_arrTime");
        lbl_arrTime->setGeometry(QRect(190, 110, 61, 21));
        lbl_arrTime->setFont(font1);
        lbl_price = new QLabel(page_3);
        lbl_price->setObjectName("lbl_price");
        lbl_price->setGeometry(QRect(390, 110, 51, 21));
        lbl_price->setFont(font1);
        lineEdit_flightID = new QLineEdit(page_3);
        lineEdit_flightID->setObjectName("lineEdit_flightID");
        lineEdit_flightID->setGeometry(QRect(70, 50, 113, 31));
        lineEdit_departure = new QLineEdit(page_3);
        lineEdit_departure->setObjectName("lineEdit_departure");
        lineEdit_departure->setGeometry(QRect(250, 50, 113, 31));
        lineEdit_destination = new QLineEdit(page_3);
        lineEdit_destination->setObjectName("lineEdit_destination");
        lineEdit_destination->setGeometry(QRect(430, 49, 113, 31));
        lineEdit_capacity = new QLineEdit(page_3);
        lineEdit_capacity->setObjectName("lineEdit_capacity");
        lineEdit_capacity->setGeometry(QRect(620, 49, 113, 31));
        lineEdit_dptTime = new QLineEdit(page_3);
        lineEdit_dptTime->setObjectName("lineEdit_dptTime");
        lineEdit_dptTime->setGeometry(QRect(70, 99, 113, 31));
        lineEdit_arrTime = new QLineEdit(page_3);
        lineEdit_arrTime->setObjectName("lineEdit_arrTime");
        lineEdit_arrTime->setGeometry(QRect(250, 99, 113, 31));
        lineEdit_price = new QLineEdit(page_3);
        lineEdit_price->setObjectName("lineEdit_price");
        lineEdit_price->setGeometry(QRect(430, 99, 113, 31));
        label_5 = new QLabel(page_3);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 150, 721, 16));
        tableFlight = new QTableWidget(page_3);
        tableFlight->setObjectName("tableFlight");
        tableFlight->setGeometry(QRect(20, 180, 721, 331));
        btn_add_3 = new QPushButton(page_3);
        btn_add_3->setObjectName("btn_add_3");
        btn_add_3->setGeometry(QRect(120, 550, 91, 31));
        btn_add_3->setFont(font);
        btn_del_3 = new QPushButton(page_3);
        btn_del_3->setObjectName("btn_del_3");
        btn_del_3->setGeometry(QRect(360, 550, 91, 31));
        btn_del_3->setFont(font);
        btn_modify_3 = new QPushButton(page_3);
        btn_modify_3->setObjectName("btn_modify_3");
        btn_modify_3->setGeometry(QRect(590, 550, 91, 31));
        btn_modify_3->setFont(font);
        stackedContent->addWidget(page_3);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        label_3 = new QLabel(page_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 20, 741, 16));
        lbl_managername = new QLabel(page_2);
        lbl_managername->setObjectName("lbl_managername");
        lbl_managername->setGeometry(QRect(50, 70, 61, 20));
        lbl_managername->setFont(font1);
        lbl_password_2 = new QLabel(page_2);
        lbl_password_2->setObjectName("lbl_password_2");
        lbl_password_2->setGeometry(QRect(310, 70, 51, 21));
        lbl_password_2->setFont(font1);
        lbl_phone_2 = new QLabel(page_2);
        lbl_phone_2->setObjectName("lbl_phone_2");
        lbl_phone_2->setGeometry(QRect(520, 70, 61, 21));
        lbl_phone_2->setFont(font1);
        lineEdit_managername = new QLineEdit(page_2);
        lineEdit_managername->setObjectName("lineEdit_managername");
        lineEdit_managername->setGeometry(QRect(110, 59, 121, 31));
        lineEdit_password_2 = new QLineEdit(page_2);
        lineEdit_password_2->setObjectName("lineEdit_password_2");
        lineEdit_password_2->setGeometry(QRect(350, 60, 121, 31));
        lineEdit_phone_2 = new QLineEdit(page_2);
        lineEdit_phone_2->setObjectName("lineEdit_phone_2");
        lineEdit_phone_2->setGeometry(QRect(580, 59, 121, 31));
        label_7 = new QLabel(page_2);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 110, 731, 16));
        tableManager = new QTableWidget(page_2);
        tableManager->setObjectName("tableManager");
        tableManager->setGeometry(QRect(20, 140, 721, 371));
        btn_add_2 = new QPushButton(page_2);
        btn_add_2->setObjectName("btn_add_2");
        btn_add_2->setGeometry(QRect(120, 550, 91, 31));
        btn_add_2->setFont(font);
        btn_del_2 = new QPushButton(page_2);
        btn_del_2->setObjectName("btn_del_2");
        btn_del_2->setGeometry(QRect(360, 550, 91, 31));
        btn_del_2->setFont(font);
        btn_modify_2 = new QPushButton(page_2);
        btn_modify_2->setObjectName("btn_modify_2");
        btn_modify_2->setGeometry(QRect(590, 550, 91, 31));
        btn_modify_2->setFont(font);
        stackedContent->addWidget(page_2);

        horizontalLayout->addWidget(widget_2);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 4);

        retranslateUi(AdminWidget);

        QMetaObject::connectSlotsByName(AdminWidget);
    } // setupUi

    void retranslateUi(QWidget *AdminWidget)
    {
        AdminWidget->setWindowTitle(QCoreApplication::translate("AdminWidget", "Form", nullptr));

        const bool __sortingEnabled = listMenu->isSortingEnabled();
        listMenu->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listMenu->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("AdminWidget", "\347\224\250\346\210\267\344\277\241\346\201\257\347\256\241\347\220\206", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = listMenu->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("AdminWidget", "\347\256\241\347\220\206\345\221\230\344\277\241\346\201\257\347\256\241\347\220\206", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = listMenu->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("AdminWidget", "\350\210\252\347\217\255\344\277\241\346\201\257\347\256\241\347\220\206", nullptr));
        listMenu->setSortingEnabled(__sortingEnabled);

        btn_del->setText(QCoreApplication::translate("AdminWidget", "\345\210\240\351\231\244", nullptr));
        btn_add->setText(QCoreApplication::translate("AdminWidget", "\346\267\273\345\212\240", nullptr));
        btn_modify->setText(QCoreApplication::translate("AdminWidget", "\344\277\256\346\224\271", nullptr));
        lbl_username->setText(QCoreApplication::translate("AdminWidget", "<html><head/><body><p><span style=\" font-size:11pt;\">\347\224\250\346\210\267\345\220\215\357\274\232</span></p></body></html>", nullptr));
        lbl_password->setText(QCoreApplication::translate("AdminWidget", "\345\257\206\347\240\201\357\274\232", nullptr));
        lbl_phone->setText(QCoreApplication::translate("AdminWidget", "<html><head/><body><p><span style=\" font-size:11pt;\">\347\224\265\350\257\235\345\217\267\347\240\201\357\274\232</span></p></body></html>", nullptr));
        label->setText(QCoreApplication::translate("AdminWidget", "-\347\224\250\346\210\267\344\277\241\346\201\257-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------", nullptr));
        label_2->setText(QCoreApplication::translate("AdminWidget", "-\346\267\273\345\212\240/\344\277\256\346\224\271\347\224\250\346\210\267\344\277\241\346\201\257------------------------------------------------------------------------------------------------------------------------------------------------------------------", nullptr));
        label_4->setText(QCoreApplication::translate("AdminWidget", "-\346\267\273\345\212\240/\344\277\256\346\224\271\350\210\252\347\217\255\344\277\241\346\201\257----------------------------------------------------------------------------------------------------------------------------------------------------------------------", nullptr));
        lbl_flightID->setText(QCoreApplication::translate("AdminWidget", "\350\210\252\347\217\255\345\217\267\357\274\232", nullptr));
        lbl_departure->setText(QCoreApplication::translate("AdminWidget", "\345\207\272\345\217\221\345\234\260\357\274\232", nullptr));
        lbl_destination->setText(QCoreApplication::translate("AdminWidget", "\347\233\256\347\232\204\345\234\260\357\274\232", nullptr));
        lbl_capicity->setText(QCoreApplication::translate("AdminWidget", "\346\240\270\350\275\275\344\272\272\346\225\260\357\274\232", nullptr));
        lbl_dptTime->setText(QCoreApplication::translate("AdminWidget", "\345\207\272\345\217\221\346\227\266\351\227\264\357\274\232", nullptr));
        lbl_arrTime->setText(QCoreApplication::translate("AdminWidget", "\345\210\260\350\276\276\346\227\266\351\227\264\357\274\232", nullptr));
        lbl_price->setText(QCoreApplication::translate("AdminWidget", "\344\273\267\346\240\274\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("AdminWidget", "-\350\210\252\347\217\255\344\277\241\346\201\257-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------", nullptr));
        btn_add_3->setText(QCoreApplication::translate("AdminWidget", "\346\267\273\345\212\240", nullptr));
        btn_del_3->setText(QCoreApplication::translate("AdminWidget", "\345\210\240\351\231\244", nullptr));
        btn_modify_3->setText(QCoreApplication::translate("AdminWidget", "\344\277\256\346\224\271", nullptr));
        label_3->setText(QCoreApplication::translate("AdminWidget", "-\346\267\273\345\212\240/\344\277\256\346\224\271\347\256\241\347\220\206\345\221\230\344\277\241\346\201\257----------------------------------------------------------------------------------------------------------------------------------------------------------------------", nullptr));
        lbl_managername->setText(QCoreApplication::translate("AdminWidget", "<html><head/><body><p>\347\256\241\347\220\206\345\221\230\345\220\215\357\274\232</p></body></html>", nullptr));
        lbl_password_2->setText(QCoreApplication::translate("AdminWidget", "<html><head/><body><p>\345\257\206\347\240\201\357\274\232</p></body></html>", nullptr));
        lbl_phone_2->setText(QCoreApplication::translate("AdminWidget", "\347\224\265\350\257\235\345\217\267\347\240\201\357\274\232", nullptr));
        label_7->setText(QCoreApplication::translate("AdminWidget", "-\347\256\241\347\220\206\345\221\230\344\277\241\346\201\257------------------------------------------------------------------------------------------------------------------------------------------------------------------------------", nullptr));
        btn_add_2->setText(QCoreApplication::translate("AdminWidget", "\346\267\273\345\212\240", nullptr));
        btn_del_2->setText(QCoreApplication::translate("AdminWidget", "\345\210\240\351\231\244", nullptr));
        btn_modify_2->setText(QCoreApplication::translate("AdminWidget", "\344\277\256\346\224\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminWidget: public Ui_AdminWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINWIDGET_H
