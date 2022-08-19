#ifndef STARPOINTFORM_H
#define STARPOINTFORM_H

#include <QWidget>
#include "mytool.h"

namespace Ui {
class StarpointForm;
}

class StarpointForm : public QWidget
{
    Q_OBJECT

public:
    explicit StarpointForm(const Info & i, QWidget *parent = nullptr);
    ~StarpointForm();

private:
    Ui::StarpointForm *ui;
    Info info;
};

#endif // STARPOINTFORM_H
