#ifndef DOWNLOAD_WIDGET_H
#define DOWNLOAD_WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <httpdownload.h>

namespace Ui {
class Download_widget;
}

class Download_widget : public QWidget
{
    Q_OBJECT

public:
    explicit Download_widget(QWidget *parent = 0);
    ~Download_widget();

private slots:
    void on_storePath_TB_clicked();

    void on_download_Bt_clicked();

private:
    Ui::Download_widget *ui;
};

#endif // DOWNLOAD_WIDGET_H
