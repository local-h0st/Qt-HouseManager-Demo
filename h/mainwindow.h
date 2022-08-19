#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mytool.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionQueryRelatives_triggered();

    void on_actionEditWorkerInfo_triggered();

    void on_actionAddWorker_triggered();

    void on_actionAddRelativeForWorker_triggered();

    void on_refreshButton_clicked();

    void on_actionShowStarpoint_triggered();

    void on_actionChooseDatabase_triggered();

private:
    Ui::MainWindow *ui;
    Database db;
    Info info;
};
#endif // MAINWINDOW_H
