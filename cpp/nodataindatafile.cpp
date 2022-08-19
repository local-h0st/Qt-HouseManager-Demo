#include "nodataindatafile.h"
#include "ui_nodataindatafile.h"

NoDataInDatafile::NoDataInDatafile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoDataInDatafile)
{
    ui->setupUi(this);
}

NoDataInDatafile::~NoDataInDatafile()
{
    delete ui;
}
