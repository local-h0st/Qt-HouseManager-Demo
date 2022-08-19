#ifndef ALLWORKERS_H
#define ALLWORKERS_H

#include <QWidget>

namespace Ui {
class allworkers;
}

class allworkers : public QWidget
{
    Q_OBJECT

public:
    explicit allworkers(QWidget *parent = nullptr);
    ~allworkers();

private:
    Ui::allworkers *ui;

//private slots:
//    void on_relative_info_btn_clicked();

};

#endif // ALLWORKERS_H
