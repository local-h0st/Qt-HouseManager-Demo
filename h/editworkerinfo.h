#ifndef EDITWORKERINFO_H
#define EDITWORKERINFO_H

#include <QWidget>
#include "mytool.h"

namespace Ui {
class EditWorkerInfo;
}

class EditWorkerInfo : public QWidget
{
    Q_OBJECT

public:
    explicit EditWorkerInfo(const Info & i,QWidget *parent = nullptr);
    ~EditWorkerInfo();

    Ui::EditWorkerInfo *ui;
    int worker_index;
    int relative_index;
    Info info;

private slots:
    void on_confirmButton_clicked();

    void on_confirmRelativeButton_clicked();

    void on_confirm_edit_button_clicked();

    void on_delete_this_relative_button_clicked();

    void on_deletaWorkerButton_clicked();

//private:
//    Ui::EditWorkerInfo *ui;
//    int worker_index;
//    int relative_index;
};

#endif // EDITWORKERINFO_H
