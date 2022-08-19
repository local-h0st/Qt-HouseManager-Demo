#ifndef ADDWORKER_H
#define ADDWORKER_H

#include <QWidget>
#include "mytool.h"

namespace Ui {
class AddWorker;
}

class AddWorker : public QWidget
{
    Q_OBJECT

public:
    explicit AddWorker(const Info & i,QWidget *parent = nullptr);
    ~AddWorker();

private slots:
    void on_add_it_button_clicked();

private:
    Ui::AddWorker *ui;
    Info info;
};

#endif // ADDWORKER_H
