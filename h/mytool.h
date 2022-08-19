#ifndef MYTOOL_H
#define MYTOOL_H

#include<QVector>
#include<QFileDialog>
#include<QString>
#include<QDebug>

class Relative{ // 亲属类
public:
    Relative(
            QString name,
            QString gender,
            QString age,
            QString appellation
    );
    Relative();// 必须有，不然QVector里面用到的话编译不能通过
    QString name;
    QString gender;
    QString age;
    QString appellation;    // 称谓关系
};

class Worker{   // 职工类
public:
    Worker(
            QString worker_id,
            QString worker_name,
            QString worker_age,
            QString date,
            QString degree,
            QString title,
            QString area,
            QString address
    );
    Worker();   // 同Relative类

    void setRelatives(QVector<Relative> relatives);
    QString worker_id;
    QString worker_name;
    QString worker_age;
    QString date;
    QString degree;
    QString title;
    QString area;
    QString address;
    QVector<Relative> relatives;
};


QString re_match_info(QString rawstr,QRegExp re);
QSet<QString> getAllMatchResults(const QString text,const QString regexp);
QVector<Relative> re_match_relatives(QString rawstr);

class Database{
public:
    Database(QString database_path);
    Database();
    void save2File(QString path);
    void addWorker(Worker worker);
    QVector<Worker> workers;
};

QString re_match_date(QString rawstr,QRegExp re, bool has_prefix=false, bool has_suffix=false);


class Info{ // 用于窗体之间传递信息
public:
    Info();
    Info(QString databasefile_path);
    Info(const Info & i);
    QString datafile_path;
};

#endif // MYTOOL_H
