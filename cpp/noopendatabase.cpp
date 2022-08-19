#include "noopendatabase.h"
#include "ui_noopendatabase.h"

NoOpenDatabase::NoOpenDatabase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoOpenDatabase)
{
    ui->setupUi(this);
}

NoOpenDatabase::~NoOpenDatabase()
{
    delete ui;
}
