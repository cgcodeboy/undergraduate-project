#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Sim_catalog *simCatalog = new Sim_catalog(this);
    simCatalog->show();
    connect(simCatalog,SIGNAL(dropClick()),this,SLOT(dropClickSlot()));
    connect(simCatalog,SIGNAL(driftClick()),this,SLOT(driftClickSlot()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 *  brief:slot function for the airplane drop ,it's connected with the dropclick signal in sim_catalog
 */
void MainWindow::dropClickSlot()
{
    ui->main_StackWidght->setCurrentIndex(0);
}

/*
 *  brief:slot function for the drift ,it's connected with the driftclick signal in sim_catalog
 */
void MainWindow::driftClickSlot()
{
    ui->main_StackWidght->setCurrentIndex(1);
}

void MainWindow::on_subject_TW_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->text(column) == "海浪高度"){

    }
}
