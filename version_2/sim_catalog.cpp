#include "sim_catalog.h"
#include "ui_sim_catalog.h"

Sim_catalog::Sim_catalog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sim_catalog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
}

Sim_catalog::~Sim_catalog()
{
    delete ui;
}

/*
 * brief: slot function for the drop_BT click
 */
void Sim_catalog::on_drop_BT_clicked()
{
    emit dropClick();
    this->close();
}

/*
 * brief: slot function for the drift_BT click
 */
void Sim_catalog::on_drift_BT_clicked()
{
    emit driftClick();
    this->close();
}
