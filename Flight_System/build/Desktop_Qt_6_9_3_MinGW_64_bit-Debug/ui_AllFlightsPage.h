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
        AllFlightsPage->setStyleSheet(QString::fromUtf8("/* ===========================================\n"
"   1. \345\205\250\345\261\200\345\255\227\344\275\223\344\270\216\350\203\214\346\231\257\350\256\276\347\275\256\n"
"   =========================================== */\n"
"QWidget {\n"
"    font-family: \"Microsoft YaHei\", \"Segoe UI\", sans-serif; /* \344\274\230\345\205\210\344\275\277\347\224\250\345\276\256\350\275\257\351\233\205\351\273\221 */\n"
"    font-size: 14px;\n"
"    color: #333333; /* \351\200\232\347\224\250\346\226\207\345\255\227\346\267\261\347\201\260\350\211\262 */\n"
"}\n"
"\n"
"/* ===========================================\n"
"   2. \351\241\266\351\203\250\347\255\233\351\200\211\346\240\217 (Top Bar)\n"
"   =========================================== */\n"
"/* \347\273\231 Frame \350\256\276\347\275\256\347\231\275\345\272\225\345\222\214\345\272\225\351\203\250\347\273\206\347\272\277\357\274\214\345\275\242\346\210\220\345\210\206\345\211\262\346\204\237 */\n"
"QFrame#topBar {\n"
"    background-color: #FFFFFF; \n"
"    border-bo"
                        "ttom: 1px solid #E5E5E5; \n"
"}\n"
"\n"
"/* ===========================================\n"
"   3. \350\276\223\345\205\245\346\241\206 (Line Edit) - \345\207\272\345\217\221\345\234\260/\347\233\256\347\232\204\345\234\260\n"
"   =========================================== */\n"
"QLineEdit {\n"
"    border: 1px solid #DCDFE6; /* \350\276\271\346\241\206\346\265\205\347\201\260 */\n"
"    border-radius: 4px;        /* \345\234\206\350\247\222 */\n"
"    padding: 0 10px;           /* \345\267\246\345\217\263\347\225\231\347\231\275\357\274\214\346\226\207\345\255\227\344\270\215\350\264\264\350\276\271 */\n"
"    background-color: #FFFFFF;\n"
"    height: 35px;              /* \345\274\272\345\210\266\351\253\230\345\272\246\357\274\214\351\230\262\346\255\242\345\244\252\346\211\201 */\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234\346\227\266\357\274\214\350\276\271\346\241\206\347\250\215\345\276\256\345\217\230\346\267\261 */\n"
"QLineEdit:hover {\n"
"    border"
                        "-color: #C0C4CC;\n"
"}\n"
"\n"
"/* \350\216\267\345\276\227\347\204\246\347\202\271\357\274\210\350\276\223\345\205\245\346\227\266\357\274\211\357\274\214\350\276\271\346\241\206\345\217\230\350\223\235 */\n"
"QLineEdit:focus {\n"
"    border: 1px solid #0078D7;\n"
"}\n"
"\n"
"/* ===========================================\n"
"   4. \346\214\211\351\222\256 (Push Button) - \346\237\245\350\257\242\346\214\211\351\222\256\n"
"   =========================================== */\n"
"QPushButton#btnSearch {\n"
"    background-color: #0078D7; /* \346\220\272\347\250\213\350\223\235/\345\276\256\350\275\257\350\223\235 */\n"
"    color: white;              /* \347\231\275\345\255\227 */\n"
"    border-radius: 4px;        /* \345\234\206\350\247\222 */\n"
"    border: none;              /* \345\216\273\346\216\211\344\270\221\351\231\213\347\232\204\350\276\271\346\241\206 */\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234\357\274\232\351\242\234\350"
                        "\211\262\345\212\240\346\267\261 */\n"
"QPushButton#btnSearch:hover {\n"
"    background-color: #0062B1;\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\214\211\344\270\213\357\274\232\351\242\234\350\211\262\346\233\264\346\267\261\357\274\214\346\250\241\346\213\237\346\214\211\345\216\213\346\204\237 */\n"
"QPushButton#btnSearch:pressed {\n"
"    background-color: #004E8C;\n"
"    padding-top: 2px; /* \347\250\215\345\276\256\344\270\213\346\262\211\344\270\200\347\202\271\347\202\271 */\n"
"    padding-left: 2px;\n"
"}\n"
"\n"
"/* ===========================================\n"
"   5. \350\243\205\351\245\260\346\200\247 Label (\351\202\243\344\270\252\347\256\255\345\244\264)\n"
"   =========================================== */\n"
"QLabel {\n"
"    color: #909399; /* \346\265\205\347\201\260\350\211\262\347\256\255\345\244\264\357\274\214\344\270\215\345\226\247\345\256\276\345\244\272\344\270\273 */\n"
"    font-weight: bold;\n"
"    font-size: 18px;\n"
"}\n"
"\n"
"/* ====================================="
                        "======\n"
"   6. \346\273\232\345\212\250\345\214\272\345\237\237 (Scroll Area)\n"
"   =========================================== */\n"
"/* \346\273\232\345\212\250\345\214\272\347\232\204\350\203\214\346\231\257\350\256\276\344\270\272\346\267\241\347\201\260\357\274\214\350\241\254\346\211\230\347\231\275\350\211\262\347\232\204\345\215\241\347\211\207 */\n"
"QScrollArea {\n"
"    border: none; /* \345\216\273\346\216\211\345\244\226\350\276\271\346\241\206 */\n"
"    background-color: #F5F7FA; \n"
"}\n"
"\n"
"/* \346\273\232\345\212\250\345\214\272\345\206\205\351\203\250\347\232\204\347\224\273\345\270\203\344\271\237\350\246\201\350\256\276\344\270\272\346\267\241\347\201\260 */\n"
"QWidget#scrollAreaWidgetContents {\n"
"    background-color: #F5F7FA;\n"
"}\n"
"\n"
"/* ===========================================\n"
"   7. (\350\277\233\351\230\266) \347\276\216\345\214\226\346\273\232\345\212\250\346\235\241 - \350\256\251\345\256\203\345\217\230\347\273\206\345\217\230\345\245\275\347\234\213\n"
"   ==="
                        "======================================== */\n"
"QScrollBar:vertical {\n"
"    border: none;\n"
"    background: #F5F7FA;\n"
"    width: 8px; /* \346\273\232\345\212\250\346\235\241\345\256\275\345\272\246\345\217\230\347\273\206 */\n"
"    margin: 0px;\n"
"}\n"
"\n"
"/* \346\273\232\345\212\250\346\235\241\351\207\214\351\235\242\347\232\204\346\273\221\345\235\227 */\n"
"QScrollBar::handle:vertical {\n"
"    background: #C0C4CC; /* \346\265\205\347\201\260\350\211\262\346\273\221\345\235\227 */\n"
"    border-radius: 4px;  /* \345\234\206\345\275\242\346\273\221\345\235\227 */\n"
"    min-height: 30px;\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\224\276\345\210\260\346\273\221\345\235\227\344\270\212\345\217\230\346\267\261 */\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: #909399;\n"
"}\n"
"\n"
"/* \351\232\220\350\227\217\344\270\212\344\270\213\347\232\204\345\260\217\347\256\255\345\244\264\346\214\211\351\222\256\357\274\214\347\216\260\344\273\243UI\351\203\275\344\270\215\347\224\250\351"
                        "\202\243\344\270\252 */\n"
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
        font.setFamilies({QString::fromUtf8("Microsoft YaHei")});
        font.setBold(true);
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
        btnSearch->setFont(font);
        dateSelector = new DateSelector(topBar);
        dateSelector->setObjectName("dateSelector");
        dateSelector->setGeometry(QRect(80, 70, 211, 31));
        label = new QLabel(topBar);
        label->setObjectName("label");
        label->setGeometry(QRect(160, 30, 41, 16));
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        scrollArea = new QScrollArea(AllFlightsPage);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setGeometry(QRect(10, 120, 571, 341));
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
