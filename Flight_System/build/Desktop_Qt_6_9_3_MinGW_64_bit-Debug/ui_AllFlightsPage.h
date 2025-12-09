/********************************************************************************
** Form generated from reading UI file 'AllFlightsPage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALLFLIGHTSPAGE_H
#define UI_ALLFLIGHTSPAGE_H

#include <DateSelector.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AllFlightsPage
{
public:
    QFrame *topBar;
    QLineEdit *lineEditDep;
    QLineEdit *lineEditArr;
    QPushButton *btnSearch;
    DateSelector *dateSelector;
    QLabel *label;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *AllFlightsPage)
    {
        if (AllFlightsPage->objectName().isEmpty())
            AllFlightsPage->setObjectName("AllFlightsPage");
        AllFlightsPage->resize(591, 472);
        AllFlightsPage->setMinimumSize(QSize(200, 40));
        AllFlightsPage->setStyleSheet(QString::fromUtf8("/* ==================== \n"
"   1. \351\241\266\351\203\250\347\255\233\351\200\211\346\240\217\346\240\267\345\274\217 \n"
"   ==================== */\n"
"QFrame#topBar {\n"
"    background-color: white;\n"
"    border-bottom: 1px solid #E0E0E0; /* \345\272\225\351\203\250\346\267\241\347\201\260\350\211\262\345\210\206\345\211\262\347\272\277 */\n"
"}\n"
"\n"
"/* \350\276\223\345\205\245\346\241\206\346\240\267\345\274\217 */\n"
"QLineEdit {\n"
"    border: 1px solid #DCDFE6;\n"
"    border-radius: 4px;\n"
"    padding: 0 10px; /* \346\226\207\345\255\227\345\206\205\350\276\271\350\267\235 */\n"
"    font-size: 14px;\n"
"    color: #333;\n"
"    background-color: white;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 1px solid #0078D7; /* \351\200\211\344\270\255\346\227\266\345\217\230\350\223\235 */\n"
"}\n"
"\n"
"/* \347\256\255\345\244\264 Label */\n"
"QLabel {\n"
"    color: #909399;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* \346\237\245\350\257\242\346\214\211\351\222\256 */\n"
"QPushButton#btnSea"
                        "rch {\n"
"    background-color: #0078D7; /* \346\220\272\347\250\213\350\223\235 */\n"
"    color: white;\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    border-radius: 4px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#btnSearch:hover {\n"
"    background-color: #005A9E; /* \346\202\254\345\201\234\346\267\261\350\223\235 */\n"
"}\n"
"\n"
"QPushButton#btnSearch:pressed {\n"
"    background-color: #004C85;\n"
"}\n"
"\n"
"/* ==================== \n"
"   2. \346\273\232\345\212\250\345\214\272\345\237\237\346\240\267\345\274\217 \n"
"   ==================== */\n"
"QScrollArea {\n"
"    background-color: #F5F7FA; /* \346\225\264\344\275\223\350\203\214\346\231\257\346\267\241\347\201\260 */\n"
"    border: none;\n"
"}\n"
"\n"
"/* \346\273\232\345\212\250\346\235\241\345\206\205\351\203\250\345\256\271\345\231\250 */\n"
"QWidget#scrollAreaWidgetContents {\n"
"    background-color: #F5F7FA; /* \347\241\256\344\277\235\351\207\214\351\235\242\344\271\237\346\230\257\346\267\241\347\201\260 */\n"
"}\n"
""
                        "\n"
"/* \346\273\232\345\212\250\346\235\241\347\276\216\345\214\226 (\345\217\257\351\200\211\357\274\214\345\216\273\346\216\211\345\260\261\346\230\257\347\263\273\347\273\237\351\273\230\350\256\244) */\n"
"QScrollBar:vertical {\n"
"    border: none;\n"
"    background: #F5F7FA;\n"
"    width: 8px;\n"
"    margin: 0px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: #C0C4CC;\n"
"    border-radius: 4px;\n"
"    min-height: 20px;\n"
"}\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    height: 0px;\n"
"}"));
        topBar = new QFrame(AllFlightsPage);
        topBar->setObjectName("topBar");
        topBar->setGeometry(QRect(10, 10, 571, 111));
        topBar->setFrameShape(QFrame::Shape::StyledPanel);
        topBar->setFrameShadow(QFrame::Shadow::Raised);
        lineEditDep = new QLineEdit(topBar);
        lineEditDep->setObjectName("lineEditDep");
        lineEditDep->setGeometry(QRect(30, 20, 121, 31));
        QFont font;
        lineEditDep->setFont(font);
        lineEditDep->setAlignment(Qt::AlignmentFlag::AlignCenter);
        lineEditArr = new QLineEdit(topBar);
        lineEditArr->setObjectName("lineEditArr");
        lineEditArr->setGeometry(QRect(210, 20, 121, 31));
        lineEditArr->setFont(font);
        lineEditArr->setAlignment(Qt::AlignmentFlag::AlignCenter);
        btnSearch = new QPushButton(topBar);
        btnSearch->setObjectName("btnSearch");
        btnSearch->setGeometry(QRect(400, 40, 100, 41));
        btnSearch->setMinimumSize(QSize(100, 40));
        QFont font1;
        font1.setBold(true);
        btnSearch->setFont(font1);
        dateSelector = new DateSelector(topBar);
        dateSelector->setObjectName("dateSelector");
        dateSelector->setGeometry(QRect(110, 70, 141, 31));
        label = new QLabel(topBar);
        label->setObjectName("label");
        label->setGeometry(QRect(160, 30, 41, 16));
        QFont font2;
        font2.setPointSize(30);
        font2.setBold(true);
        label->setFont(font2);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        scrollArea = new QScrollArea(AllFlightsPage);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setGeometry(QRect(10, 120, 571, 341));
        scrollArea->setFrameShape(QFrame::Shape::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 571, 341));
        scrollArea->setWidget(scrollAreaWidgetContents);

        retranslateUi(AllFlightsPage);

        QMetaObject::connectSlotsByName(AllFlightsPage);
    } // setupUi

    void retranslateUi(QWidget *AllFlightsPage)
    {
        AllFlightsPage->setWindowTitle(QCoreApplication::translate("AllFlightsPage", "Form", nullptr));
        lineEditDep->setPlaceholderText(QCoreApplication::translate("AllFlightsPage", "\345\207\272\345\217\221\345\237\216\345\270\202", nullptr));
        lineEditArr->setPlaceholderText(QCoreApplication::translate("AllFlightsPage", "\345\210\260\350\276\276\345\237\216\345\270\202", nullptr));
        btnSearch->setText(QCoreApplication::translate("AllFlightsPage", "\346\220\234\347\264\242", nullptr));
        label->setText(QCoreApplication::translate("AllFlightsPage", "\342\207\204", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AllFlightsPage: public Ui_AllFlightsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALLFLIGHTSPAGE_H
