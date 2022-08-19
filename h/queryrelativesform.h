#ifndef QUERYRELATIVESFORM_H
#define QUERYRELATIVESFORM_H

#include <QWidget>
#include "mytool.h"

namespace Ui {
class QueryRelativesForm;
}

class QueryRelativesForm : public QWidget
{
    Q_OBJECT

public:
    explicit QueryRelativesForm(const Info & i, QWidget *parent = nullptr);
    ~QueryRelativesForm();

private slots:
    void on_confirmButton_clicked();
//    void onCellChanged();

private:
    Ui::QueryRelativesForm *ui;
    Database db;
    int worker_index;
    Info info;
};

#endif // QUERYRELATIVESFORM_H
