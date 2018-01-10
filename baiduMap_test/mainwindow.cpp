#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->map_View->setUrl(QUrl("file:///E:/my_program/undergraduate/baiduMap_test/map.html"));
    //QUrl("E:\my_program\undergraduate\baiduMap_test\map.html"

    ui->map_View->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
