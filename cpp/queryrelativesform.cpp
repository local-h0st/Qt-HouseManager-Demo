#include "queryrelativesform.h"
#include "ui_queryrelativesform.h"
#include <QStandardItemModel>
#include "mytool.h"


QueryRelativesForm::QueryRelativesForm(const Info & i, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryRelativesForm),
    info(i)
{
    ui->setupUi(this);
    ui->comboBox->clear();  // 先清空
    this->db = Database(this->info.datafile_path);
    for(int i=0;i<db.workers.count();++i){
        ui->comboBox->addItem(db.workers.at(i).worker_id + " " + db.workers.at(i).worker_name);
    }

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 不可编辑
}

QueryRelativesForm::~QueryRelativesForm()
{
    delete ui;
}

void QueryRelativesForm::on_confirmButton_clicked()
{
    worker_index = ui->comboBox->currentIndex();
    QVector<Relative> relatives = db.workers.at(worker_index).relatives;
    // mainwindow里面列出worker的代码可以复用
    // 设置表头
    QStandardItemModel* model = new QStandardItemModel();
    QStringList headerText = QObject::trUtf8("姓名,性别,年龄,称谓").simplified().split(",");
    model->setHorizontalHeaderLabels(headerText);
    // 隐藏行号
    ui->tableView->verticalHeader()->hide();
    // 赋值
    for(int i=0;i<relatives.count();++i){
        model->setItem(i,0,new QStandardItem(relatives.at(i).name));
        model->setItem(i,1,new QStandardItem(relatives.at(i).gender));
        model->setItem(i,2,new QStandardItem(relatives.at(i).age));
        model->setItem(i,3,new QStandardItem(relatives.at(i).appellation));
    }

    ui->tableView->setModel(model); // model设置完了，可以应用给table了
}

