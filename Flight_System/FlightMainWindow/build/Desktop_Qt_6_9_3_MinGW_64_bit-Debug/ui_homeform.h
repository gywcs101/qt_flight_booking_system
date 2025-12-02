/********************************************************************************
** Form generated from reading UI file 'homeform.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOMEFORM_H
#define UI_HOMEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include "bannerwidget.h"

QT_BEGIN_NAMESPACE

class Ui_homeForm
{
public:
    BannerWidget *widgetBanner;

    void setupUi(QWidget *homeForm)
    {
        if (homeForm->objectName().isEmpty())
            homeForm->setObjectName("homeForm");
        homeForm->resize(751, 621);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(homeForm->sizePolicy().hasHeightForWidth());
        homeForm->setSizePolicy(sizePolicy);
        widgetBanner = new BannerWidget(homeForm);
        widgetBanner->setObjectName("widgetBanner");
        widgetBanner->setGeometry(QRect(10, 0, 731, 181));
        sizePolicy.setHeightForWidth(widgetBanner->sizePolicy().hasHeightForWidth());
        widgetBanner->setSizePolicy(sizePolicy);

        retranslateUi(homeForm);

        QMetaObject::connectSlotsByName(homeForm);
    } // setupUi

    void retranslateUi(QWidget *homeForm)
    {
        homeForm->setWindowTitle(QCoreApplication::translate("homeForm", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class homeForm: public Ui_homeForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMEFORM_H
