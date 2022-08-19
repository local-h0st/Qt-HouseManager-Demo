#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "allworkers.h"
#include "queryrelativesform.h"
#include "editworkerinfo.h"
#include "addworker.h"
#include "addrelativeforworker.h"
#include "nodataindatafile.h"
#include "starpointform.h"
#include "noopendatabase.h"
#include<QStandardItemModel>
#include<QPushButton>
#include <QDateTime>
#include "mytool.h"


#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->info.datafile_path = "nil";
//    this->info.datafile_path = "../HouseManager/WorkersData";
//    this->db = Database(this->info.datafile_path); // 打开数据文件

    // 设置表头
    QStandardItemModel* model = new QStandardItemModel();
    QStringList headerText = QObject::trUtf8("工号,姓名,年龄,入职日期,学历,职称,现有住房面积,地址").simplified().split(",");
    model->setHorizontalHeaderLabels(headerText);
    // 隐藏行号
    ui->tableView->verticalHeader()->hide();
    // 赋值
//    for(int i=0;i<db.workers.count();++i){
//        model->setItem(i,0,new QStandardItem(db.workers.at(i).worker_id));
//        model->setItem(i,1,new QStandardItem(db.workers.at(i).worker_name));

//        int birth_year = re_match_date(db.workers.at(i).worker_age,QRegExp("[0-9][0-9][0-9][0-9]")).toInt();
//        int current_year = QDateTime::currentDateTime().date().year();
//        model->setItem(i,2,new QStandardItem(QString::number(current_year - birth_year)));

//        model->setItem(i,3,new QStandardItem(db.workers.at(i).date));
//        model->setItem(i,4,new QStandardItem(db.workers.at(i).degree));
//        model->setItem(i,5,new QStandardItem(db.workers.at(i).title));
//        model->setItem(i,6,new QStandardItem(db.workers.at(i).area));
//        model->setItem(i,7,new QStandardItem(db.workers.at(i).address));
//    }

    ui->tableView->setModel(model); // model设置完了，可以应用给table了

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 不可编辑

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionQueryRelatives_triggered()
{
    if(this->info.datafile_path=="nil"){
        (new NoOpenDatabase())->show();
    }
    else if(Database(this->info.datafile_path).workers.count()==0){
        (new NoDataInDatafile())->show();
    }
    else{
        (new QueryRelativesForm(this->info))->show();
    }

}

void MainWindow::on_actionEditWorkerInfo_triggered()
{
    if(this->info.datafile_path=="nil"){
        (new NoOpenDatabase())->show();
    }
    else if(Database(this->info.datafile_path).workers.count()==0){
        (new NoDataInDatafile())->show();
    }
    else{
        EditWorkerInfo * editworkerinfo_window = new EditWorkerInfo(this->info);
        editworkerinfo_window->setAttribute(Qt::WA_DeleteOnClose);
        connect(editworkerinfo_window, &QWidget::destroyed, this, &MainWindow::on_refreshButton_clicked);
        editworkerinfo_window->show();
    }
}

void MainWindow::on_actionAddWorker_triggered()
{
    if(this->info.datafile_path=="nil"){
        (new NoOpenDatabase())->show();
    }
    else {
        AddWorker * addworker_window = new AddWorker(this->info);
        addworker_window->setAttribute(Qt::WA_DeleteOnClose);
        connect(addworker_window, &QWidget::destroyed, this, &MainWindow::on_refreshButton_clicked);
        addworker_window->show();
    }

}

void MainWindow::on_actionAddRelativeForWorker_triggered()
{
    if(this->info.datafile_path=="nil"){
        (new NoOpenDatabase())->show();
    }
    else if(Database(this->info.datafile_path).workers.count()==0){
        (new NoDataInDatafile())->show();
    }
    else{
        (new AddRelativeForWorker(this->info))->show();
    }

}

void MainWindow::on_refreshButton_clicked()
{
    ui->tableView->model()->removeRows(0,ui->tableView->model()->rowCount());   // 清空
    // 复用构造函数
    this->db = Database(this->info.datafile_path); // 打开数据文件
    QStandardItemModel* model = new QStandardItemModel();
    QStringList headerText = QObject::trUtf8("工号,姓名,年龄,入职日期,学历,职称,现有住房面积,地址").simplified().split(",");
    model->setHorizontalHeaderLabels(headerText);
    ui->tableView->verticalHeader()->hide();
    for(int i=0;i<db.workers.count();++i){
        model->setItem(i,0,new QStandardItem(db.workers.at(i).worker_id));
        model->setItem(i,1,new QStandardItem(db.workers.at(i).worker_name));

        int birth_year = re_match_date(db.workers.at(i).worker_age,QRegExp("[0-9][0-9][0-9][0-9]")).toInt();
        int current_year = QDateTime::currentDateTime().date().year();
        model->setItem(i,2,new QStandardItem(QString::number(current_year - birth_year)));

        model->setItem(i,3,new QStandardItem(db.workers.at(i).date));
        model->setItem(i,4,new QStandardItem(db.workers.at(i).degree));
        model->setItem(i,5,new QStandardItem(db.workers.at(i).title));
        model->setItem(i,6,new QStandardItem(db.workers.at(i).area));
        model->setItem(i,7,new QStandardItem(db.workers.at(i).address));
    }
    ui->tableView->setModel(model);
}

void MainWindow::on_actionShowStarpoint_triggered()
{
    if(this->info.datafile_path=="nil"){
        (new NoOpenDatabase())->show();
    }
    else if(Database(this->info.datafile_path).workers.count()==0){
        (new NoDataInDatafile())->show();
    }
    else{
        (new StarpointForm(this->info))->show();
    }
}

void MainWindow::on_actionChooseDatabase_triggered()
{
    QString db_name = QFileDialog::getOpenFileName(this,QStringLiteral("选择职工数据库文件"));
    this->info.datafile_path = db_name;
    this->on_refreshButton_clicked();
}
