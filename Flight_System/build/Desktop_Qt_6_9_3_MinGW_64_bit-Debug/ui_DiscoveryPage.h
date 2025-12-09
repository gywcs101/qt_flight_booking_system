/********************************************************************************
** Form generated from reading UI file 'DiscoveryPage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISCOVERYPAGE_H
#define UI_DISCOVERYPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DiscoveryPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;

    void setupUi(QWidget *DiscoveryPage)
    {
        if (DiscoveryPage->objectName().isEmpty())
            DiscoveryPage->setObjectName("DiscoveryPage");
        DiscoveryPage->resize(682, 488);
        verticalLayout = new QVBoxLayout(DiscoveryPage);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(DiscoveryPage);
        label->setObjectName("label");
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\215\216\346\226\207\347\220\245\347\217\200")});
        font.setPointSize(18);
        label->setFont(font);

        verticalLayout->addWidget(label);

        scrollArea = new QScrollArea(DiscoveryPage);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 668, 451));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setObjectName("gridLayout");
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(DiscoveryPage);

        QMetaObject::connectSlotsByName(DiscoveryPage);
    } // setupUi

    void retranslateUi(QWidget *DiscoveryPage)
    {
        label->setText(QCoreApplication::translate("DiscoveryPage", "\345\217\221\347\216\260", nullptr));
        (void)DiscoveryPage;
    } // retranslateUi

};

namespace Ui {
    class DiscoveryPage: public Ui_DiscoveryPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISCOVERYPAGE_H
