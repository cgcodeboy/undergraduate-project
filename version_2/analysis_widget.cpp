﻿#include "analysis_widget.h"
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
    if(!ui->fileName_TV->selectionModel()->hasSelection()){
        QMessageBox::warning(this,"warning","choose!");
        return;
    }
    if(ui->dataType_CM->currentIndex() == 0){
        int ncid;
        int uID, vID, timeID, depthID,latID,lonID;
        int x, y, retval;
        size_t time_len,depth_len,lat_len,lon_len;
        int time_offset = 0, lat_offset = 0, lon_offset = 0;

        QString fileName = ui->fileName_TV->selectionModel()->currentIndex().data().toString();
        string openName = ui->filePath_LE->text().toStdString()+"\\"+fileName.toStdString();
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

        ofstream outfile(ui->storePath_LE->text().toStdString()+"\current_20140307.txt",ios_base::app);
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
        QModelIndexList modelList = ui->fileName_TV->selectionModel()->selectedRows();
        QString storeNameTime = modelList.at(0).data().toString().mid(6,8);
        ofstream outfile(ui->storePath_LE->text().toStdString()+"/wind_"+storeNameTime.toStdString()+".txt",ios_base::app);
        if (!outfile.is_open())
        {
            qDebug()<<"open error!";
            return;
        }

        const int NX = 3264;
        const int NY = 82;

        for(int i = 0;i<modelList.size();i++){
            QString pathName = ui->filePath_LE->text()+"/"+modelList.at(i).data().toString();
            int ncid, windDirID, windSpeedID, latID, lonID;
            float *windDirData = new float[NX*NY], *windSpeedData = new float[NX*NY];
            int *latData = new int[NX*NY], *lonData = new int[NX*NY];

            int x, y, retval;

            if ((retval = nc_open(pathName.toStdString().c_str(), NC_NOWRITE, &ncid)))
                ERR(retval);

            //get wind direction data
            if ((retval = nc_inq_varid(ncid, "wind_dir", &windDirID)))
                ERR(retval);
            if ((retval = nc_get_var_float(ncid, windDirID, &windDirData[0])))
                ERR(retval);

            //get wind speed data
            if ((retval = nc_inq_varid(ncid, "wind_speed", &windSpeedID)))
                ERR(retval);
            if ((retval = nc_get_var_float(ncid, windSpeedID, &windSpeedData[0])))
                ERR(retval);

            if ((retval = nc_inq_varid(ncid, "lon", &lonID)))
                ERR(retval);
            if ((retval = nc_get_var_int(ncid, lonID, &lonData[0])))
                ERR(retval);

            //get lon data
            if ((retval = nc_inq_varid(ncid, "lat", &latID)))
                ERR(retval);
            if ((retval = nc_get_var_int(ncid, latID, &latData[0])))
                ERR(retval);

            /* write the data. */
            //outfile << "latitude longitude  wind_dir  wind_speed\n";
            for (x = 0; x < NX; x++){
                for (y = 0; y < NY; y++){
                    float lat = *(latData + x* NY + y) *pow(10, -5);
                    float lon = *(lonData + x* NY + y) *pow(10, -5);
                    if (lat<20 && lat>-35 && lon < 115 && lon>40){
                        outfile<< lat << " " << lon << " ";
                        if (*(windDirData + x* NY + y) >0 && *(windDirData + x* NY + y) < 3600)
                            outfile << *(windDirData + x* NY + y) * 0.1 << " ";
                        else{
                            outfile << "nan ";
                        }

                        if (*(windSpeedData + x* NY + y) > 0 && *(windSpeedData + x* NY + y) < 5000)
                            outfile << *(windSpeedData + x* NY + y)* 0.01 << " ";
                        else{
                            outfile << "nan ";
                        }
                        outfile << "\n";
                    }
                }
            }

            if ((retval = nc_close(ncid)))
                ERR(retval);
        }
        outfile.close();
    }
}
