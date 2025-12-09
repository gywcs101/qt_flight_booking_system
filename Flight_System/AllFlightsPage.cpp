#include "AllFlightsPage.h"
#include "ui_AllFlightsPage.h"

AllFlightsPage::AllFlightsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AllFlightsPage)
{
    ui->setupUi(this);
}

AllFlightsPage::~AllFlightsPage()
{
    delete ui;
}
