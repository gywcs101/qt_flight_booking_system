/********************************************************************************
** Form generated from reading UI file 'RegisterWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERWIDGET_H
#define UI_REGISTERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterWidget
{
public:
    QLabel *lbl_username;
    QLabel *lbl_password;
    QLabel *lbl_confirmPassword;
    QLineEdit *lineEdit_username;
    QLineEdit *lineEdit_password;
    QLineEdit *lineEdit_confirmPassword;
    QPushButton *btn_register;
    QPushButton *btn_cancel;
    QLineEdit *lineEdit_phone;
    QLabel *lbl_password_2;

    void setupUi(QWidget *RegisterWidget)
    {
        if (RegisterWidget->objectName().isEmpty())
            RegisterWidget->setObjectName("RegisterWidget");
        RegisterWidget->resize(268, 334);
        RegisterWidget->setMinimumSize(QSize(268, 334));
        RegisterWidget->setMaximumSize(QSize(268, 334));
        lbl_username = new QLabel(RegisterWidget);
        lbl_username->setObjectName("lbl_username");
        lbl_username->setGeometry(QRect(40, 110, 51, 21));
        QFont font;
        font.setPointSize(10);
        lbl_username->setFont(font);
        lbl_password = new QLabel(RegisterWidget);
        lbl_password->setObjectName("lbl_password");
        lbl_password->setGeometry(QRect(40, 150, 51, 21));
        lbl_password->setFont(font);
        lbl_confirmPassword = new QLabel(RegisterWidget);
        lbl_confirmPassword->setObjectName("lbl_confirmPassword");
        lbl_confirmPassword->setGeometry(QRect(30, 190, 61, 21));
        lbl_confirmPassword->setFont(font);
        lineEdit_username = new QLineEdit(RegisterWidget);
        lineEdit_username->setObjectName("lineEdit_username");
        lineEdit_username->setGeometry(QRect(90, 100, 121, 31));
        lineEdit_username->setFont(font);
        lineEdit_password = new QLineEdit(RegisterWidget);
        lineEdit_password->setObjectName("lineEdit_password");
        lineEdit_password->setGeometry(QRect(90, 140, 121, 31));
        lineEdit_password->setFont(font);
        lineEdit_confirmPassword = new QLineEdit(RegisterWidget);
        lineEdit_confirmPassword->setObjectName("lineEdit_confirmPassword");
        lineEdit_confirmPassword->setGeometry(QRect(90, 180, 121, 31));
        lineEdit_confirmPassword->setFont(font);
        btn_register = new QPushButton(RegisterWidget);
        btn_register->setObjectName("btn_register");
        btn_register->setGeometry(QRect(70, 270, 56, 31));
        btn_register->setFont(font);
        btn_cancel = new QPushButton(RegisterWidget);
        btn_cancel->setObjectName("btn_cancel");
        btn_cancel->setGeometry(QRect(160, 270, 56, 31));
        btn_cancel->setFont(font);
        lineEdit_phone = new QLineEdit(RegisterWidget);
        lineEdit_phone->setObjectName("lineEdit_phone");
        lineEdit_phone->setGeometry(QRect(90, 220, 121, 31));
        lineEdit_phone->setFont(font);
        lbl_password_2 = new QLabel(RegisterWidget);
        lbl_password_2->setObjectName("lbl_password_2");
        lbl_password_2->setGeometry(QRect(40, 230, 51, 21));
        lbl_password_2->setFont(font);

        retranslateUi(RegisterWidget);

        QMetaObject::connectSlotsByName(RegisterWidget);
    } // setupUi

    void retranslateUi(QWidget *RegisterWidget)
    {
        RegisterWidget->setWindowTitle(QCoreApplication::translate("RegisterWidget", "\346\263\250\345\206\214", nullptr));
        lbl_username->setText(QCoreApplication::translate("RegisterWidget", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        lbl_password->setText(QCoreApplication::translate("RegisterWidget", "\345\257\206   \347\240\201\357\274\232", nullptr));
        lbl_confirmPassword->setText(QCoreApplication::translate("RegisterWidget", "\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        btn_register->setText(QCoreApplication::translate("RegisterWidget", "\346\263\250\345\206\214", nullptr));
        btn_cancel->setText(QCoreApplication::translate("RegisterWidget", "\345\217\226\346\266\210", nullptr));
        lbl_password_2->setText(QCoreApplication::translate("RegisterWidget", "\347\224\265  \350\257\235\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterWidget: public Ui_RegisterWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERWIDGET_H
