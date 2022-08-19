#ifndef NODATAINDATAFILE_H
#define NODATAINDATAFILE_H

#include <QWidget>

namespace Ui {
class NoDataInDatafile;
}

class NoDataInDatafile : public QWidget
{
    Q_OBJECT

public:
    explicit NoDataInDatafile(QWidget *parent = nullptr);
    ~NoDataInDatafile();

private:
    Ui::NoDataInDatafile *ui;
};

#endif // NODATAINDATAFILE_H
