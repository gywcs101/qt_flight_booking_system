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
#include <QtWidgets/QComboBox>
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
    QComboBox *comboBox;
    QLabel *lbl_password_2;
    QPushButton *btn_register;

    void setupUi(QWidget *LoginWidget)
    {
        if (LoginWidget->objectName().isEmpty())
            LoginWidget->setObjectName("LoginWidget");
        LoginWidget->resize(268, 334);
        LoginWidget->setMinimumSize(QSize(268, 334));
        LoginWidget->setMaximumSize(QSize(268, 334));
        btn_login = new QPushButton(LoginWidget);
        btn_login->setObjectName("btn_login");
        btn_login->setGeometry(QRect(60, 240, 56, 31));
        pushButton_2 = new QPushButton(LoginWidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(150, 240, 56, 31));
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
        label->setGeometry(QRect(80, 291, 71, 21));
        comboBox = new QComboBox(LoginWidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(110, 180, 121, 31));
        lbl_password_2 = new QLabel(LoginWidget);
        lbl_password_2->setObjectName("lbl_password_2");
        lbl_password_2->setGeometry(QRect(40, 180, 61, 31));
        lbl_password_2->setFont(font);
        btn_register = new QPushButton(LoginWidget);
        btn_register->setObjectName("btn_register");
        btn_register->setGeometry(QRect(144, 287, 93, 28));
        btn_register->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btn_register->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: none;              /* \345\216\273\346\216\211\350\276\271\346\241\206 */\n"
"    background-color: transparent; /* \350\203\214\346\231\257\351\200\217\346\230\216 */\n"
"    color: #007bff;           /* \346\226\207\345\255\227\351\242\234\350\211\262\357\274\232\350\223\235\350\211\262 */\n"
"    text-align: left;          /* \346\226\207\345\255\227\345\267\246\345\257\271\351\275\220 (\345\217\257\351\200\211) */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    color: #0056b3;           /* \351\274\240\346\240\207\346\202\254\345\201\234\346\227\266\351\242\234\350\211\262\345\217\230\346\267\261 */\n"
"    text-decoration: underline; /* \351\274\240\346\240\207\346\202\254\345\201\234\346\227\266\345\212\240\344\270\213\345\210\222\347\272\277\357\274\214\346\233\264\346\234\211\351\223\276\346\216\245\347\232\204\346\204\237\350\247\211 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    color: #004494;           /* \346\214\211\344\270\213\346\227\266\351\242\234\350\211\262\346\233"
                        "\264\346\267\261 */\n"
"}"));

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
        comboBox->setItemText(0, QCoreApplication::translate("LoginWidget", "\347\224\250\346\210\267", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("LoginWidget", "\347\256\241\347\220\206\345\221\230", nullptr));

        lbl_password_2->setText(QCoreApplication::translate("LoginWidget", "\350\272\253    \344\273\275\357\274\232", nullptr));
        btn_register->setText(QCoreApplication::translate("LoginWidget", "\347\253\213\345\215\263\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWidget: public Ui_LoginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWIDGET_H
