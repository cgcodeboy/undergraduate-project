#ifndef ANALYSIS_WIDGET_H
#define ANALYSIS_WIDGET_H

/*
 * Qt header file
 */
#include <QWidget>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QModelIndex>
#include <QDebug>

/*
 * user header file
 */
#include <netcdf.h>
#include <fstream>

using namespace std;

#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); system("pause");}

namespace Ui {
class Analysis_widget;
}

class Analysis_widget : public QWidget
{
    Q_OBJECT

public:
    explicit Analysis_widget(QWidget *parent = 0);
    ~Analysis_widget();

private slots:
    void on_filePath_TB_clicked();

    void on_storePath_TB_clicked();

    void on_analysis_BT_clicked();

private:
    Ui::Analysis_widget *ui;
};

#endif // ANALYSIS_WIDGET_H
