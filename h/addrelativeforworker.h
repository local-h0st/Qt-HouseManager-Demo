#ifndef ADDRELATIVEFORWORKER_H
#define ADDRELATIVEFORWORKER_H

#include <QWidget>
#include "mytool.h"

namespace Ui {
class AddRelativeForWorker;
}

class AddRelativeForWorker : public QWidget
{
    Q_OBJECT

public:
    explicit AddRelativeForWorker(const Info & i, QWidget *parent = nullptr);
    ~AddRelativeForWorker();

private slots:
    void on_addButton_clicked();

private:
    Ui::AddRelativeForWorker *ui;
    int worker_index;
    Info info;
};

#endif // ADDRELATIVEFORWORKER_H
