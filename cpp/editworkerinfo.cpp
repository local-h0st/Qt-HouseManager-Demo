#include "editworkerinfo.h"
#include "ui_editworkerinfo.h"
#include <QRegExpValidator>
#include "mytool.h"
#define NIL -1
#define DELETED -2

// 自定义辅助函数
void refresh_and_show_window(EditWorkerInfo * p){
    p->ui->chooseWorkerBox->clear();  // 先清空
    Database db(p->info.datafile_path);
    for(int i=0;i<db.workers.count();++i){
        p->ui->chooseWorkerBox->addItem(db.workers.at(i).worker_id + " " + db.workers.at(i).worker_name);
    }
    p->ui->workerIDEdit->setText("");
    p->ui->WorkerNameEdit->setText("");
    p->ui->WorkerDegreecomboBox->clear();
    p->ui->WorkerTitlecomboBox->clear();
    p->ui->WorkerAreaEdit->setText("");
    p->ui->WorkerAddressEdit->setText("");
    p->ui->chooseRelativeBox->clear();
    p->ui->relativeNameEdit->setText("");
    p->ui->RelativeGendercomboBox->clear();
    p->ui->relativeAgeEdit->setText("");
    p->ui->relativeAppellationEdit->setText("");
    p->ui->confirm_edit_button->setEnabled(false);
    p->ui->confirmRelativeButton->setEnabled(false);
    p->ui->delete_this_relative_button->setEnabled(false);
    p->ui->alertLabel->setText("");
    p->ui->alertRelativelabel->setText("");
    if(Database(p->info.datafile_path).workers.count()==0){
        p->ui->confirmButton->setEnabled(false);
        p->ui->deletaWorkerButton->setEnabled(false);
    }
}
//QString re_match_date(QString rawstr,QRegExp re, bool has_prefix=false, bool has_suffix=false){
//    if(rawstr.indexOf(re)>=0){
//        QString str = re.cap(0);
//        if(has_prefix){
//            str = str.mid(1);
//        }
//        if(has_suffix){
//            str = str.mid(0,str.length()-1);
//        }

//        return str;
//    }
//    else{
//        return "nil";   // 没匹配到，默认为空nil
//    }
//} 我放到mytools里面去了

// 定义结束

EditWorkerInfo::EditWorkerInfo(const Info & i, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWorkerInfo),
    info(i)
{
    ui->setupUi(this);
    refresh_and_show_window(this);

    // 输入限制
    ui->relativeAgeEdit->setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]*$")));
    ui->WorkerAreaEdit->setValidator(new QRegExpValidator(QRegExp("([1-9][0-9]*$)|(0)")));
}

EditWorkerInfo::~EditWorkerInfo()
{
    delete ui;
}


void EditWorkerInfo::on_confirmButton_clicked()
{
    ui->confirm_edit_button->setEnabled(true);

    relative_index = NIL;   // 表示没有选择
    worker_index = ui->chooseWorkerBox->currentIndex();

    Worker current_worker = Database(this->info.datafile_path).workers.at(worker_index);
    ui->workerIDEdit->setText(current_worker.worker_id);
    ui->WorkerNameEdit->setText(current_worker.worker_name);

    QString birthdate = current_worker.worker_age;
    birthdate += "#";    // 为了方便正则匹配 刚好可以稍微改一下之前的re_match_info太妙了
    QString birthyear = re_match_date(birthdate,QRegExp("[0-9][0-9][0-9][0-9]"));
    QString birthmonth = re_match_date(birthdate,QRegExp("/[^/]*/"),true,true);
    QString birthday = re_match_date(birthdate,QRegExp("/[^/]*#"),true,true);
    ui->birthdateEdit->setDate(QDate(
                                           birthyear.toInt(),
                                           birthmonth.toInt(),
                                           birthday.toInt()
                                           ));

    QString date = current_worker.date;
    date += "#";    // 为了方便正则匹配 刚好可以稍微改一下之前的re_match_info太妙了
    QString year = re_match_date(date,QRegExp("[0-9][0-9][0-9][0-9]"));
    QString month = re_match_date(date,QRegExp("/[^/]*/"),true,true);
    QString day = re_match_date(date,QRegExp("/[^/]*#"),true,true);
    ui->NewWorkerDatedateEdit->setDate(QDate(
                                           year.toInt(),
                                           month.toInt(),
                                           day.toInt()
                                           ));

    ui->WorkerDegreecomboBox->clear();
    ui->WorkerDegreecomboBox->addItem(current_worker.degree);
    if(current_worker.degree=="本科"){
        ui->WorkerDegreecomboBox->addItem("硕士");
        ui->WorkerDegreecomboBox->addItem("博士");
    }
    else if(current_worker.degree=="硕士"){
        ui->WorkerDegreecomboBox->addItem("本科");
        ui->WorkerDegreecomboBox->addItem("博士");
    }
    else{
        ui->WorkerDegreecomboBox->addItem("本科");
        ui->WorkerDegreecomboBox->addItem("硕士");
    }   // 丑陋至极

    ui->WorkerTitlecomboBox->clear();
    ui->WorkerTitlecomboBox->addItem(current_worker.title);
    if(current_worker.title=="助教"){
        ui->WorkerTitlecomboBox->addItem("讲师");
        ui->WorkerTitlecomboBox->addItem("副教授");
        ui->WorkerTitlecomboBox->addItem("教授");
    }
    else if(current_worker.title=="讲师"){
        ui->WorkerTitlecomboBox->addItem("助教");
        ui->WorkerTitlecomboBox->addItem("副教授");
        ui->WorkerTitlecomboBox->addItem("教授");
    }
    else if(current_worker.title=="副教授"){
        ui->WorkerTitlecomboBox->addItem("助教");
        ui->WorkerTitlecomboBox->addItem("讲师");
        ui->WorkerTitlecomboBox->addItem("教授");
    }
    else{
        ui->WorkerTitlecomboBox->addItem("助教");
        ui->WorkerTitlecomboBox->addItem("讲师");
        ui->WorkerTitlecomboBox->addItem("副教授");
    }


    ui->WorkerAreaEdit->setText(current_worker.area);
    ui->WorkerAddressEdit->setText(current_worker.address);


    ui->chooseRelativeBox->clear();
    for(int i=0;i<current_worker.relatives.count();++i){
        ui->chooseRelativeBox->addItem(current_worker.relatives.at(i).name);
    }
    if(current_worker.relatives.count()==0){
        ui->confirmRelativeButton->setEnabled(false);
    }
    else{
        ui->confirmRelativeButton->setEnabled(true);
    }

}

void EditWorkerInfo::on_confirmRelativeButton_clicked()
{
    ui->delete_this_relative_button->setEnabled(true);
    relative_index = ui->chooseRelativeBox->currentIndex();
    Relative current_relative = Database(this->info.datafile_path).workers.at(worker_index).relatives.at(relative_index);

    // 填充数据
    ui->relativeNameEdit->setText(current_relative.name);
    ui->RelativeGendercomboBox->clear();
    ui->RelativeGendercomboBox->addItem(current_relative.gender);
    if(current_relative.gender=="男"){
        ui->RelativeGendercomboBox->addItem("女");
        ui->RelativeGendercomboBox->addItem("LGBTQ");
    }
    else if(current_relative.gender=="女"){
        ui->RelativeGendercomboBox->addItem("男");
        ui->RelativeGendercomboBox->addItem("LGBTQ");
    }
    else {
        ui->RelativeGendercomboBox->addItem("男");
        ui->RelativeGendercomboBox->addItem("女");
    }   // 太丑陋了
    ui->relativeAgeEdit->setText(current_relative.age);
    ui->relativeAppellationEdit->setText(current_relative.appellation);
}

void EditWorkerInfo::on_confirm_edit_button_clicked()
{
    Database new_db;
    Database db(this->info.datafile_path);

    for(int i=0;i<db.workers.count();++i){
        if(i!=worker_index){    // 不是当前正在修改的worker，直接复制一份过来
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
            worker.setRelatives(db.workers.at(i).relatives);
            new_db.workers.push_back(worker);
        }
        else {  // 是当前正在修改的worker
            // 先读入worker info
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
                        ui->NewWorkerDatedateEdit->text(),
                        ui->WorkerDegreecomboBox->currentText(),
                        ui->WorkerTitlecomboBox->currentText(),
                        ui->WorkerAreaEdit->text(),
                        address
                        );
            // 再处理relative的情况
            if(relative_index == NIL){  // relative没有被修改
                worker.setRelatives(db.workers.at(worker_index).relatives);
            }
            else if(relative_index == DELETED){ // 刷新数据库
                db = Database(this->info.datafile_path);
            }
            else {  // 被修改
                QVector<Relative> new_relatives;
                for(int i=0;i<db.workers.at(worker_index).relatives.count();++i){
                    if(i!=relative_index){  // 不是修改的那个
                        new_relatives.push_back(db.workers.at(worker_index).relatives.at(i));
                    }
                    else{   // 是被修改的那个
                        Relative new_relative(
                                    ui->relativeNameEdit->text(),
                                    ui->RelativeGendercomboBox->currentText(),
                                    ui->relativeAgeEdit->text(),
                                    ui->relativeAppellationEdit->text()
                                    );
                        new_relatives.push_back(new_relative);
                    }
                }
                worker.setRelatives(new_relatives);
            }
            // worker全部组装完毕
            new_db.workers.push_back(worker);
        }
    }   // new_db组装完毕

    new_db.save2File(this->info.datafile_path);

    refresh_and_show_window(this);
    ui->alertLabel->setText("保存成功");
}

void EditWorkerInfo::on_delete_this_relative_button_clicked()
{
    // 可以复用on_confirm_edit_button_clicked的
    Database new_db;
    Database db(this->info.datafile_path);

    for(int i=0;i<db.workers.count();++i){
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
            QVector<Relative> new_relatives;
            for(int j=0;j<db.workers.at(worker_index).relatives.count();++j){
                if(j!=relative_index){  // 不是要删除的那个
                    new_relatives.push_back(db.workers.at(i).relatives.at(j));
                }
                worker.setRelatives(new_relatives);
            }
        }
        // worker全部组装完毕
        new_db.workers.push_back(worker);

    }   // new_db组装完毕


    new_db.save2File(this->info.datafile_path);
    ui->alertRelativelabel->setEnabled(true);
    ui->alertRelativelabel->setText("删掉了");


    relative_index = DELETED;
    ui->relativeNameEdit->setText("");
    ui->RelativeGendercomboBox->clear();
    ui->relativeAgeEdit->setText("");
    ui->relativeAppellationEdit->setText("");
    ui->chooseRelativeBox->clear();
    db = Database(this->info.datafile_path);   // 刷新一下数据库
    for(int i=0;i<db.workers.at(worker_index).relatives.count();++i){
        ui->chooseRelativeBox->addItem(db.workers.at(worker_index).relatives.at(i).name);
    }
    if(db.workers.at(worker_index).relatives.count()==0){
        ui->delete_this_relative_button->setEnabled(false);
    }
}

void EditWorkerInfo::on_deletaWorkerButton_clicked()
{
    worker_index = ui->chooseWorkerBox->currentIndex();
    // 复用之前的代码
    Database new_db;
    Database db(this->info.datafile_path);
    for(int i=0;i<db.workers.count();++i){
        if(i!=worker_index){
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
            worker.setRelatives(db.workers.at(i).relatives);
            new_db.workers.push_back(worker);
        }
    }   // new_db组装完毕
    new_db.save2File(this->info.datafile_path);
    worker_index = DELETED;

    refresh_and_show_window(this);
    ui->alertLabel->setText("职工删掉了");
}
