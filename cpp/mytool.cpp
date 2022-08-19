#include "mytool.h"


Relative::Relative(
            QString name,
            QString gender,
            QString age,
            QString appellation
    ){
        this->name = name;
        this->gender = gender;
        this->age = age;
        this->appellation = appellation;
    }
Relative::Relative(){ // 必须有，不然QVector里面用到的话编译不能通过

    }



Worker::Worker(
            QString worker_id,
            QString worker_name,
            QString worker_age,
            QString date,
            QString degree,
            QString title,
            QString area,
            QString address
    ){
        this->worker_id = worker_id;
        this->worker_name = worker_name;
        this->worker_age = worker_age;
        this->date = date;
        this->degree = degree;
        this->title = title;
        this->area = area;
        this->address = address;
    }
Worker::Worker(){   // 同Relative类

    }
void Worker::setRelatives(QVector<Relative> relatives){
        this->relatives = relatives;
    }




QString re_match_info(QString rawstr,QRegExp re){
    if(rawstr.indexOf(re)>=0){
        QString str = re.cap(0);    // 可以匹配到字符串worker_name:aaa因为这个是最后一条记录所以被显示出来了
        QRegExp r(":.*");   // 去掉冒号(:)之前的
        str.indexOf(r);
        str = r.cap(0);
        str = str.mid(1);   // 截去冒号(:)
        return str;
    }
    else{
        return "nil";   // 没匹配到，默认为空nil
    }
}


QSet<QString> getAllMatchResults(const QString text,const QString regexp){
    // 用于获取全部的匹配结果，其实是用来辅助re_match_relaives（）也可以直接用它获取一整个文件的全部记录并返回set，但是代买要重构，就算了
    QSet<QString>resultSet;
    QRegExp rx(regexp);
    int pos=0;
    while((pos=rx.indexIn(text,pos))!=-1){
        pos+=rx.matchedLength();
        QString result=rx.cap(0);
        resultSet<<result;
//        qDebug()<<"found"<<result;
    }
    return resultSet;
}

QVector<Relative> re_match_relatives(QString rawstr){
    // info是指 name gender age appellation四个里面的任意一个
    QVector<Relative> relatives;
    QSet<QString> relatives_rawset = getAllMatchResults(rawstr,"\\{[^\\}]*");    // 获取到所有大括号
    QSetIterator<QString> i(relatives_rawset);  // Iter遍历
    while(i.hasNext()){
        QString relative_str = i.next();
        const Relative one_relative(
                    re_match_info(relative_str,QRegExp("name:[^&]*")),
                    re_match_info(relative_str,QRegExp("gender:[^&]*")),
                    re_match_info(relative_str,QRegExp("age:[^&]*")),
                    re_match_info(relative_str,QRegExp("appellation:[^&]*"))
                    );
        relatives<<one_relative;
    }
    return relatives;
}


Database::Database(QString database_path){
        // read database file
        // 单次解析文件[start]到[end]
        // 然后利用解析出来的数据构建一个Worker实例
        // 然后直接添加到private的workers里面
        // 要是C++有序列化和反序列化的话多方便啊
        QFile dbfile(database_path);
        if(!dbfile.exists()){
            qDebug()<<"[err]database file does not exists";
            throw;
        }
        dbfile.open(QFile::ReadOnly);
        while (!dbfile.atEnd()) {
            // 读取一行，其实开头的[start]和结尾的[end]没有用，但是为了结构明了还是加上去了
            QByteArray array = dbfile.readLine();
            QString linestr = QString(array);   // linestr就是这一行的内容
            // Qt正则匹配，比如匹配 worker_name:balabala|拿到结果，再匹配:balabala|拿到结果，再去掉:和|拿到最终数据
            Worker worker(  // 对当前这一行的信息创建Worker对象
                        re_match_info(linestr,QRegExp("worker_id:[^|]*")),
                        re_match_info(linestr,QRegExp("worker_name:[^|]*")),
                        re_match_info(linestr,QRegExp("worker_age:[^|]*")),
                        re_match_info(linestr,QRegExp("date:[^|]*")),
                        re_match_info(linestr,QRegExp("degree:[^|]*")),
                        re_match_info(linestr,QRegExp("title:[^|]*")),
                        re_match_info(linestr,QRegExp("area:[^|]*")),
                        re_match_info(linestr,QRegExp("address:[^|]*"))
                        );
            worker.setRelatives(re_match_relatives(linestr));   // 读取并存储亲属信息
            this->workers<<(worker);                // 当前对象添加到database
        }
        dbfile.close(); // 文件在循环中全部读取完毕，关闭文件
    }

Database::Database(){

}

void Database::save2File(QString path){
    QFile new_db_file(path);
    if(!new_db_file.open(QIODevice::WriteOnly|QIODevice::Text)){    // 会清空当下文件内容
        qDebug()<<"file open error";
        throw;
    }

    QStringList strs;
    for(int i=0;i<this->workers.count();++i){
        Worker current_worker = this->workers.at(i);
        QString linestr = "[start]worker_id:" + current_worker.worker_id + "|" + "worker_name:" + current_worker.worker_name + "|" + "worker_age:" + current_worker.worker_age + "|" + "date:" + current_worker.date + "|" + "worker_degree:" + current_worker.degree + "|" + "title:" + current_worker.title + "|" + "area:" + current_worker.area + "|" + "address:" + current_worker.address + "|";
        if(current_worker.relatives.count() == 0){
            linestr += "[end]";
        }
        else {
            for(int j=0;j<current_worker.relatives.count();++j){
                linestr += "{name:" + current_worker.relatives.at(j).name + "&gender:" + current_worker.relatives.at(j).gender + "&age:" + current_worker.relatives.at(j).age + "&appellation:" + current_worker.relatives.at(j).appellation + "}";
            }
            linestr += "[end]";
        }
//        qDebug()<<linestr;    // 格式正确
        strs.append(linestr);
    }

    // 写入文件
    QTextStream db_file_stream(&new_db_file);
    db_file_stream.setCodec("UTF-8");
    for(QString line: strs){
        db_file_stream << line << "\n";
    }

}

void Database::addWorker(Worker worker){
    this->workers.push_back(worker);
}


QString re_match_date(QString rawstr,QRegExp re, bool has_prefix, bool has_suffix){
    if(rawstr.indexOf(re)>=0){
        QString str = re.cap(0);
        if(has_prefix){
            str = str.mid(1);
        }
        if(has_suffix){
            str = str.mid(0,str.length()-1);
        }

        return str;
    }
    else{
        return "nil";   // 没匹配到，默认为空nil
    }
}


Info::Info(){

}

Info::Info(QString datafile_path){
    this->datafile_path = datafile_path;
}

Info::Info(const Info & i){
    this->datafile_path = i.datafile_path;
}
