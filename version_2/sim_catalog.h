#ifndef SIM_CATALOG_H
#define SIM_CATALOG_H

#include <QWidget>

namespace Ui {
class Sim_catalog;
}

class Sim_catalog : public QWidget
{
    Q_OBJECT

public:
    explicit Sim_catalog(QWidget *parent = 0);
    ~Sim_catalog();

private:
    Ui::Sim_catalog *ui;

signals:
    void dropClick();   //a signal for drop_BT click , when it emit ,it will connect with the dropclickslot in mainwindow
    void driftClick();   //a signal for drift_BT click , when it emit ,it will connect with the driftclickslot in mainwindow

private slots:
    void on_drop_BT_clicked();
    void on_drift_BT_clicked();
};

#endif // SIM_CATALOG_H
