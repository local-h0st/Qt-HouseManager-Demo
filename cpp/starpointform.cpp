#include "starpointform.h"
#include "ui_starpointform.h"
#include <QStandardItem>
#include <QDateTime>

StarpointForm::StarpointForm(const Info & i, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StarpointForm),
    info(i)
{
    ui->setupUi(this);
    // 复用mainwindow的构造函数
    Database db(this->info.datafile_path);
    // 设置表头
    QStandardItemModel* model = new QStandardItemModel();
    QStringList headerText = QObject::trUtf8("工号,姓名,积分统计").simplified().split(",");
    model->setHorizontalHeaderLabels(headerText);
    // 隐藏行号
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSortingEnabled(true);
    // 赋值
    for(int i=0;i<db.workers.count();++i){
        Worker current_worker = db.workers.at(i);
        model->setItem(i,0,new QStandardItem(current_worker.worker_id));
        model->setItem(i,1,new QStandardItem(current_worker.worker_name));

        float starpoint = 0;
        // 职称分
        if(current_worker.title=="教授"){
            starpoint += 20;
        }
        else if(current_worker.title=="副教授"){
            starpoint += 16;
        }
        else if(current_worker.title=="讲师"){
            starpoint += 12;
        }
        else{   // 助教
            starpoint += 8;
        }
        // 工龄分
        QDateTime current_date_time = QDateTime::currentDateTime();
        int current_year = current_date_time.date().year();
        int current_month = current_date_time.date().month();
        QString date = current_worker.date; //复用edit里面的
        date += "#";
        int year = re_match_date(date,QRegExp("[0-9][0-9][0-9][0-9]")).toInt();
        int month = re_match_date(date,QRegExp("/[^/]*/"),true,true).toInt();
        int delta_year = 0,delta_month = 0;
        if(current_month>month){
            delta_year = current_year - year;
            delta_month = current_month - month;
        }
        else if(current_month<month){
            delta_year = current_year - year - 1;
            delta_month = 12 + current_month - month;
        }
        else{
            delta_year = current_year - year;
            delta_month = 0;
        }
        starpoint += 0.5*delta_year + 0.0416*delta_month;
        // 年龄分
        QString birthdate = current_worker.worker_age;
        birthdate += "#";
        int birthyear = re_match_date(birthdate,QRegExp("[0-9][0-9][0-9][0-9]")).toInt();
        int birthmonth = re_match_date(birthdate,QRegExp("/[^/]*/"),true,true).toInt();
        if(current_month>birthmonth){
            delta_year = current_year - birthyear;
            delta_month = current_month - birthmonth;
        }
        else if(current_month<birthmonth){
            delta_year = current_year - year - 1;
            delta_month = 12 + current_month - month;
        }
        starpoint += 0.1*delta_year + 0.0083*delta_month;
        // 学龄分
        if(current_worker.degree=="博士"){
            starpoint += 4.5;
        }
        else if(current_worker.degree=="硕士"){
            starpoint += 3.25;
        }
        else{
            starpoint += 2;
        }
        // 现住房分
        starpoint += 0.1*current_worker.area.toInt();
        // 家庭成员分
        starpoint += 2*current_worker.relatives.count();


        model->setItem(i,2,new QStandardItem(QString::number(starpoint)));
    }

    ui->tableView->setModel(model); // model设置完了，可以应用给table了
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 不可编辑
}

StarpointForm::~StarpointForm()
{
    delete ui;
}
