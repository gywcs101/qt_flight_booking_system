/********************************************************************************
** Form generated from reading UI file 'UserCenter.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERCENTER_H
#define UI_USERCENTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserCenter
{
public:
    QLabel *Avatar;
    QLabel *label;
    QPushButton *btnEdit;
    QPushButton *btnPass;
    QPushButton *btnLogout;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit_3;
    QLineEdit *phoneNumber;
    QDateEdit *Birthday;
    QLineEdit *phoneNumber_2;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;

    void setupUi(QWidget *UserCenter)
    {
        if (UserCenter->objectName().isEmpty())
            UserCenter->setObjectName("UserCenter");
        UserCenter->resize(747, 617);
        QFont font;
        font.setPointSize(15);
        UserCenter->setFont(font);
        Avatar = new QLabel(UserCenter);
        Avatar->setObjectName("Avatar");
        Avatar->setGeometry(QRect(70, 90, 201, 191));
        Avatar->setPixmap(QPixmap(QString::fromUtf8("../../../Pictures/Screenshots/oo9qhAAmlDAnz7EzCIfhgUzMAeA9TiAIENNJNd~noop.jpeg")));
        Avatar->setScaledContents(true);
        label = new QLabel(UserCenter);
        label->setObjectName("label");
        label->setGeometry(QRect(156, 60, 71, 21));
        label->setFont(font);
        btnEdit = new QPushButton(UserCenter);
        btnEdit->setObjectName("btnEdit");
        btnEdit->setGeometry(QRect(140, 430, 91, 41));
        btnPass = new QPushButton(UserCenter);
        btnPass->setObjectName("btnPass");
        btnPass->setGeometry(QRect(340, 430, 91, 41));
        btnLogout = new QPushButton(UserCenter);
        btnLogout->setObjectName("btnLogout");
        btnLogout->setGeometry(QRect(540, 430, 91, 41));
        layoutWidget = new QWidget(UserCenter);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(410, 50, 167, 281));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit_3 = new QLineEdit(layoutWidget);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setMinimumSize(QSize(144, 24));

        verticalLayout->addWidget(lineEdit_3);

        phoneNumber = new QLineEdit(layoutWidget);
        phoneNumber->setObjectName("phoneNumber");
        phoneNumber->setMinimumSize(QSize(144, 24));

        verticalLayout->addWidget(phoneNumber);

        Birthday = new QDateEdit(layoutWidget);
        Birthday->setObjectName("Birthday");
        Birthday->setMinimumSize(QSize(109, 24));

        verticalLayout->addWidget(Birthday);

        phoneNumber_2 = new QLineEdit(layoutWidget);
        phoneNumber_2->setObjectName("phoneNumber_2");
        phoneNumber_2->setMinimumSize(QSize(144, 24));

        verticalLayout->addWidget(phoneNumber_2);

        layoutWidget1 = new QWidget(UserCenter);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(320, 70, 152, 241));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(81, 20));
        label_2->setFont(font);

        verticalLayout_2->addWidget(label_2);

        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName("label_3");
        label_3->setFont(font);

        verticalLayout_2->addWidget(label_3);

        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName("label_4");

        verticalLayout_2->addWidget(label_4);

        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName("label_5");

        verticalLayout_2->addWidget(label_5);


        retranslateUi(UserCenter);

        QMetaObject::connectSlotsByName(UserCenter);
    } // setupUi

    void retranslateUi(QWidget *UserCenter)
    {
        UserCenter->setWindowTitle(QCoreApplication::translate("UserCenter", "Form", nullptr));
        Avatar->setText(QString());
        label->setText(QCoreApplication::translate("UserCenter", "\345\244\264\345\203\217", nullptr));
        btnEdit->setText(QCoreApplication::translate("UserCenter", "\347\274\226\350\276\221\350\265\204\346\226\231", nullptr));
        btnPass->setText(QCoreApplication::translate("UserCenter", "\344\277\256\346\224\271\345\257\206\347\240\201", nullptr));
        btnLogout->setText(QCoreApplication::translate("UserCenter", "\351\200\200\345\207\272\347\231\273\345\275\225", nullptr));
        label_2->setText(QCoreApplication::translate("UserCenter", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("UserCenter", "\346\211\213\346\234\272\345\217\267\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("UserCenter", "\347\224\237\346\227\245\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("UserCenter", "\345\223\210\345\244\253\345\270\201\344\275\231\351\242\235\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserCenter: public Ui_UserCenter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERCENTER_H
