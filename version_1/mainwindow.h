#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
 * Qt头文件
 */
#include <QMainWindow>
#include <QDebug>

/*
 * 引用头文件
 */
#include <sim_catalog.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void dropClickSlot();
    void driftClickSlot();
};

#endif // MAINWINDOW_H
