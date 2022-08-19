#include "allworkers.h"
#include "ui_allworkers.h"
#include<QStandardItemModel>
#include<QPushButton>
#include "mytool.h"

allworkers::allworkers(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::allworkers)
{
    ui->setupUi(this);
    ui->tableView->resize(this->width(),this->height());    // 铺满当前窗口

    //    QString db_name = QFileDialog::getOpenFileName(this,QStringLiteral("选择职工数据库文件"));
    //    Database db(db_name);
    Database db("../HouseManager/WorkersData"); // 打开数据文件

    // 设置表头
    QStandardItemModel* model = new QStandardItemModel();
    QStringList headerText = QObject::trUtf8("worker_id,worker_name,worker_age,date,degree,title,area,address").simplified().split(",");
    model->setHorizontalHeaderLabels(headerText);

    // 赋值
    for(int i=0;i<db.workers.count();++i){
        model->setItem(i,0,new QStandardItem(db.workers.at(i).worker_id));
        model->setItem(i,1,new QStandardItem(db.workers.at(i).worker_name));
        model->setItem(i,2,new QStandardItem(db.workers.at(i).worker_age));
        model->setItem(i,3,new QStandardItem(db.workers.at(i).date));
        model->setItem(i,4,new QStandardItem(db.workers.at(i).degree));
        model->setItem(i,5,new QStandardItem(db.workers.at(i).title));
        model->setItem(i,6,new QStandardItem(db.workers.at(i).area));
        model->setItem(i,7,new QStandardItem(db.workers.at(i).address));
//        // 放置查看亲属按钮
//        QPushButton* relative_info_button = new QPushButton("See Relative Info");
//        connect(relative_info_button,SIGNAL(click()),this,SLOT(on_relative_info_btn_clicked()));
//        ui->tableView->setIndexWidget(model->index(i,8),relative_info_button);  // TODO 显示不出来是怎么回事我靠
//        既然这样那就改变逻辑，把查看亲属单独设置为一个功能，分开实现
    }

    ui->tableView->setModel(model); // model设置完了，可以应用给table了

}

allworkers::~allworkers()
{
    delete ui;
}



//void allworkers::on_relative_info_btn_clicked(){
//    // TODO 弹窗，新表格看亲属内容
//}
