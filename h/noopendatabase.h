#ifndef NOOPENDATABASE_H
#define NOOPENDATABASE_H

#include <QWidget>

namespace Ui {
class NoOpenDatabase;
}

class NoOpenDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit NoOpenDatabase(QWidget *parent = nullptr);
    ~NoOpenDatabase();

private:
    Ui::NoOpenDatabase *ui;
};

#endif // NOOPENDATABASE_H
