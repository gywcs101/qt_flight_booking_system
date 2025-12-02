/********************************************************************************
** Form generated from reading UI file 'LoginWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWIDGET_H
#define UI_LOGINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWidget
{
public:
    QPushButton *btn_login;
    QPushButton *pushButton_2;
    QLabel *lbl_username;
    QLabel *lbl_password;
    QLineEdit *lineEdit_username;
    QLineEdit *lineEdit_password;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *LoginWidget)
    {
        if (LoginWidget->objectName().isEmpty())
            LoginWidget->setObjectName("LoginWidget");
        LoginWidget->resize(268, 334);
        LoginWidget->setMinimumSize(QSize(268, 334));
        LoginWidget->setMaximumSize(QSize(268, 334));
        btn_login = new QPushButton(LoginWidget);
        btn_login->setObjectName("btn_login");
        btn_login->setGeometry(QRect(60, 200, 56, 31));
        pushButton_2 = new QPushButton(LoginWidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(140, 200, 56, 31));
        lbl_username = new QLabel(LoginWidget);
        lbl_username->setObjectName("lbl_username");
        lbl_username->setGeometry(QRect(40, 100, 61, 31));
        QFont font;
        font.setPointSize(11);
        lbl_username->setFont(font);
        lbl_password = new QLabel(LoginWidget);
        lbl_password->setObjectName("lbl_password");
        lbl_password->setGeometry(QRect(40, 140, 61, 31));
        lbl_password->setFont(font);
        lineEdit_username = new QLineEdit(LoginWidget);
        lineEdit_username->setObjectName("lineEdit_username");
        lineEdit_username->setGeometry(QRect(110, 100, 121, 31));
        lineEdit_password = new QLineEdit(LoginWidget);
        lineEdit_password->setObjectName("lineEdit_password");
        lineEdit_password->setGeometry(QRect(110, 140, 121, 31));
        label = new QLabel(LoginWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(80, 290, 61, 21));
        label_2 = new QLabel(LoginWidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(140, 290, 51, 21));

        retranslateUi(LoginWidget);

        QMetaObject::connectSlotsByName(LoginWidget);
    } // setupUi

    void retranslateUi(QWidget *LoginWidget)
    {
        LoginWidget->setWindowTitle(QCoreApplication::translate("LoginWidget", "\347\231\273\345\275\225", nullptr));
        btn_login->setText(QCoreApplication::translate("LoginWidget", "\347\231\273\345\275\225", nullptr));
        pushButton_2->setText(QCoreApplication::translate("LoginWidget", "\345\217\226\346\266\210", nullptr));
        lbl_username->setText(QCoreApplication::translate("LoginWidget", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        lbl_password->setText(QCoreApplication::translate("LoginWidget", "\345\257\206    \347\240\201\357\274\232", nullptr));
        label->setText(QCoreApplication::translate("LoginWidget", "\346\262\241\346\234\211\350\264\246\345\217\267\357\274\237", nullptr));
        label_2->setText(QCoreApplication::translate("LoginWidget", "<html><head/><body><p><span style=\" color:#0000ff;\">\347\253\213\345\215\263\346\263\250\345\206\214</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWidget: public Ui_LoginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWIDGET_H
