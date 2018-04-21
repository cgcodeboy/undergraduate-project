#include "analysis_widget.h"
#include "ui_analysis_widget.h"

Analysis_widget::Analysis_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Analysis_widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);

    ui->fileName_TV->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fileName_TV->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

Analysis_widget::~Analysis_widget()
{
    delete ui;
}

void Analysis_widget::on_filePath_TB_clicked()
{
    QString dirStr = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                       "/home",
                                                       QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);

    ui->filePath_LE->setText(dirStr);

    QDir dir(dirStr);
    QStringList filter;
    filter<<"*.nc";
    QStringList fileList = dir.entryList(filter);

    QStandardItemModel *model = new QStandardItemModel;
    for(QString str:fileList){
        QStandardItem *item = new QStandardItem(str);
        model->appendRow(item);
    }

    ui->fileName_TV->setModel(model);
}

void Analysis_widget::on_storePath_TB_clicked()
{
    QString storePathStr = QFileDialog::getExistingDirectory(this, tr("Store Directory"),
                                                             "/home",
                                                             QFileDialog::ShowDirsOnly
                                                             | QFileDialog::DontResolveSymlinks);
    ui->storePath_LE->setText(storePathStr);
}

void Analysis_widget::on_analysis_BT_clicked()
{
    if(ui->storePath_LE->text().isEmpty()||!ui->fileName_TV->selectionModel()->hasSelection()){
        QMessageBox::warning(this,tr("提示信息"),"保存路径为空或未点选待分析项!");
        return;
    }
    if(ui->dataType_CM->currentText() == "海浪数据"){
        int ncid;
        int uID, vID, timeID, depthID,latID,lonID;
        int x, y, retval;
        size_t time_len,depth_len,lat_len,lon_len;
        int time_offset = 0, lat_offset = 0, lon_offset = 0;

        QString fileName = ui->fileName_TV->selectionModel()->currentIndex().data().toString();
        string openName = ui->filePath_LE->text().toStdString()+"\\"+fileName.toStdString();
        qDebug()<<QString(openName.c_str());
        if ((retval = nc_open(openName.c_str(), NC_NOWRITE, &ncid)))
            ERR(retval);

        if ((retval = nc_inq_varid(ncid, "u", &uID)))
            ERR(retval);
        if ((retval = nc_inq_varid(ncid, "v", &vID)))
            ERR(retval);

        if ((retval = nc_inq_dimid(ncid, "time", &timeID)))
            ERR(retval);
        if ((retval = nc_inq_dimlen(ncid, timeID, &time_len)))
            ERR(retval);
        if ((retval = nc_inq_dimid(ncid, "depth", &depthID)))
            ERR(retval);
        if ((retval = nc_inq_dimlen(ncid, depthID, &depth_len)))
            ERR(retval);
        if ((retval = nc_inq_dimid(ncid, "latitude", &latID)))
            ERR(retval);
        if ((retval = nc_inq_dimlen(ncid, latID, &lat_len)))
            ERR(retval);
        if ((retval = nc_inq_dimid(ncid, "longitude", &lonID)))
            ERR(retval);
        if ((retval = nc_inq_dimlen(ncid, lonID, &lon_len)))
            ERR(retval);

        double *u_data = new double[time_len * lat_len * lon_len];
        double *v_data = new double[time_len * lat_len * lon_len];
        double *lat_data = new double[lat_len];
        double *lon_data = new double[lon_len];
        int *time_data = new int[time_len];


        if ((retval = nc_get_var_double(ncid, uID, &u_data[0])))
            ERR(retval);
        if ((retval = nc_get_var_double(ncid, vID, &v_data[0])))
            ERR(retval);
        if ((retval = nc_get_var_double(ncid, latID, &lat_data[0])))
            ERR(retval);
        if ((retval = nc_get_var_double(ncid, lonID, &lon_data[0])))
            ERR(retval);
        if ((retval = nc_get_var_int(ncid, timeID, &time_data[0])))
            ERR(retval);

        ofstream outfile(ui->storePath_LE->text().toStdString()+"\current_20140307.txt");
        if (!outfile.is_open())
        {
            printf("file open error!\n");
            return;
        }

        for (time_offset = 13; time_offset < 18; time_offset++){
            outfile << "time_offset:" << time_offset << "\n";
            for (lat_offset = 0; lat_offset < lat_len; lat_offset++){
                for (lon_offset = 0; lon_offset < lon_len; lon_offset++){
                    double latCur = *(lat_data + lat_offset);
                    double lonCur = *(lon_data + lon_offset);
                    if (lonCur < 110 && lonCur >80 && latCur < 0 && latCur > -35){
                        outfile << *(time_data + time_offset) << " "\
                                <<latCur<< " " << lonCur <<" "\
                               << *(u_data + time_offset * lat_len * lon_len + lat_offset * lon_len + lon_offset) << " "\
                               << *(v_data + time_offset * lat_len * lon_len + lat_offset * lon_len + lon_offset)<< "\n";
                    }
                }
            }
        }

        delete []u_data,v_data,lat_data,lon_data,time_data;

        outfile.close();
        if ((retval = nc_close(ncid)))
            ERR(retval);
    }
    else{
        QMessageBox::question(this,"询问",tr("风场数据是离散数据,请注意将一天的数据全部点选,分析结果将合并为一个文件"),QMessageBox::Yes,QMessageBox::No);
    }
}
