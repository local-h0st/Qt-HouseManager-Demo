#include "addrelativeforworker.h"
#include "ui_addrelativeforworker.h"
#include <QRegExpValidator>
#include "mytool.h"

AddRelativeForWorker::AddRelativeForWorker(const Info & i,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddRelativeForWorker),
    info(i)
{
    ui->setupUi(this);
    // 下拉框的可复用之前的
    ui->chooseWorkerBox->clear();  // 先清空
    Database db(this->info.datafile_path);
    for(int i=0;i<db.workers.count();++i){
        ui->chooseWorkerBox->addItem(db.workers.at(i).worker_id + " " + db.workers.at(i).worker_name);
    }
    // 输入限制
    ui->relativeAgeEdit->setValidator(new QRegExpValidator(QRegExp("([1-9][0-9]*$)")));
    ui->RelativeGendercomboBox->addItem("男");
    ui->RelativeGendercomboBox->addItem("女");
    ui->RelativeGendercomboBox->addItem("LGBTQ");
}

AddRelativeForWorker::~AddRelativeForWorker()
{
    delete ui;
}

// 好好看看这个https://cloud.tencent.com/developer/article/1732749

void AddRelativeForWorker::on_addButton_clicked()
{
    worker_index = ui->chooseWorkerBox->currentIndex();
    if(
            ui->relativeNameEdit->text()==""
            ||ui->relativeAgeEdit->text()==""
            ||ui->relativeAppellationEdit->text()==""
        ){
        ui->alertlabel->setText("信息不能为空");
    }
    else{   // 全部验证完毕，没有问题可以添加
        Relative new_relative(
                    ui->relativeNameEdit->text(),
                    ui->RelativeGendercomboBox->currentText(),
                    ui->relativeAgeEdit->text(),
                    ui->relativeAppellationEdit->text()
                    );

        // 可以复用
        Database new_db;
        Database db(this->info.datafile_path);
        for(int i=0;i<db.workers.count();++i){  // 如果数据库是空的那根本不会进入这个循环
            // 直接复制一份过来
            Worker worker(
                        db.workers.at(i).worker_id,
                        db.workers.at(i).worker_name,
                        db.workers.at(i).worker_age,
                        db.workers.at(i).date,
                        db.workers.at(i).degree,
                        db.workers.at(i).title,
                        db.workers.at(i).area,
                        db.workers.at(i).address
                        );

            if(i!=worker_index){
                worker.setRelatives(db.workers.at(i).relatives);
            }
            else {
                QVector<Relative> new_relatives = db.workers.at(i).relatives;
                new_relatives.push_back(new_relative);
                worker.setRelatives(new_relatives);
            }

            // worker全部组装完毕
            new_db.workers.push_back(worker);

        }   // new_db组装完毕


        new_db.save2File(this->info.datafile_path);

        ui->alertlabel->setText("添加好了");


        // 刷新一下界面
        ui->relativeNameEdit->setText("");
        ui->relativeAgeEdit->setText("");
        ui->relativeAppellationEdit->setText("");


    }
}
