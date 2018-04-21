#include "download_widget.h"
#include "ui_download_widget.h"

Download_widget::Download_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Download_widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
}

Download_widget::~Download_widget()
{
    delete ui;
}

void Download_widget::on_storePath_TB_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    ui->storePath_LE->setText(dir);
}

void Download_widget::on_download_Bt_clicked()
{
    HttpDownload *inner_download = new HttpDownload;
    inner_download->setDownloadUrl(ui->dataURL_LE->text());
    inner_download->setStorePath(ui->storePath_LE->text());
    inner_download->startDownload();
}
