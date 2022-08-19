#include "addworker.h"
#include "ui_addworker.h"
#include <QRegExpValidator>
#include "mytool.h"

AddWorker::AddWorker(const Info & i, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddWorker),
    info(i)
{
    ui->setupUi(this);
    // 设输入限制
    ui->DegreecomboBox->addItem("本科");
    ui->DegreecomboBox->addItem("硕士");
    ui->DegreecomboBox->addItem("博士");
    ui->TitlecomboBox->addItem("助教");
    ui->TitlecomboBox->addItem("讲师");
    ui->TitlecomboBox->addItem("副教授");
    ui->TitlecomboBox->addItem("教授");
    ui->WorkerAreaEdit->setValidator(new QRegExpValidator(QRegExp("([1-9][0-9]*$)|(0)")));

}

AddWorker::~AddWorker()
{
    delete ui;
}

void AddWorker::on_add_it_button_clicked()
{
    if(
            ui->workerIDEdit->text().isEmpty()
            ||ui->WorkerNameEdit->text().isEmpty()
            ||ui->WorkerAreaEdit->text().isEmpty()
        ){
        ui->label->setText("工号姓名年龄面积不能有空的啊");
    }
    else if(ui->WorkerAreaEdit->text()!="0"&&ui->WorkerAddressEdit->text().isEmpty()){
        ui->label->setText("有住房面积的话怎么会没有地址呢");
    }
    else {  // 全部check完毕，保存
//        QString date = ui->WorkerdateEdit->date().toString();  这个会显示处理过的内容，但是我不需要
        QString address;
        if(ui->WorkerAreaEdit->text()=="0"){
            address = "无";
        }
        else {
            address = ui->WorkerAddressEdit->text();
        }
        Worker worker(
                    ui->workerIDEdit->text(),
                    ui->WorkerNameEdit->text(),
                    ui->birthdateEdit->text(),
                    ui->WorkerdateEdit->text(),
                    ui->DegreecomboBox->currentText(),
                    ui->TitlecomboBox->currentText(),
                    ui->WorkerAreaEdit->text(),
                    address
                    );
        qDebug()<<this->info.datafile_path;
        Database new_db(this->info.datafile_path);
        new_db.addWorker(worker);
        new_db.save2File(this->info.datafile_path);
        ui->label->setText("添加好了");
        ui->workerIDEdit->setText("");
        ui->WorkerNameEdit->setText("");
        ui->WorkerAreaEdit->setText("");
        ui->WorkerAddressEdit->setText("");
    }
}
