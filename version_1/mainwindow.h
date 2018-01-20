#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
 * Qt头文件
 */
#include <QMainWindow>
#include <QDebug>
#include <QTreeWidget>
#include <QtWebKit>
#include <QMessageBox>
#include <QtMath>

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
    void routeClickSlot();
    void on_subject_TW_itemClicked(QTreeWidgetItem *item, int column);
    void on_parameter_TW_currentChanged(int index);
    void on_windDirection_DIAL_valueChanged(int value);
    void on_currentDirection_DIAL_valueChanged(int value);
    void on_parameter_TW_tabBarClicked(int index);
    void on_calculate_PB_clicked();
};

#endif // MAINWINDOW_H
