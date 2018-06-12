#include "possynthesis_widget.h"
#include "ui_possynthesis_widget.h"

PosSynthesis_Widget::PosSynthesis_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PosSynthesis_Widget)
{
    ui->setupUi(this);
}

PosSynthesis_Widget::~PosSynthesis_Widget()
{
    delete ui;
}

void PosSynthesis_Widget::on_synthesis_BT_clicked()
{
    if(ui->beforeLat_LE->text().isEmpty()||ui->beforeLon_LE->text().isEmpty()){
        QMessageBox::warning(this,"warn information","please input the lat and lon");
        return;
    }
    if(ui->dropX_LE->text().isEmpty()||ui->dropY_LE->text().isEmpty()){
        QMessageBox::warning(this,"warn information","please input drop information");
        return;
    }
    float before_Lat = ui->beforeLat_LE->text().toFloat();
    float before_Lon = ui->beforeLon_LE->text().toFloat();

    float dropX = ui->dropX_LE->text().toFloat();
    float dropY = ui->dropY_LE->text().toFloat();

    Position beforePos = calculateellipse2plane(before_Lat,before_Lon);
    float x = beforePos.x + dropX;
    float y = beforePos.y + dropY;

    LatLon afterPos = Calculateplane2ellipse(x,y);

    ui->afterLat_LE->setText(QString::number(afterPos.lat));
    ui->afterLon_LE->setText(QString::number(afterPos.lon));
}

void PosSynthesis_Widget::on_mapShow_BT_clicked()
{
    if(ui->showLat_LE->text().isEmpty()||ui->showLon_LE->text().isEmpty()){
        QMessageBox::warning(this,"warn information","please input lat and lon information need to show in map");
        return;
    }
    emit showLatAndLon(ui->showLat_LE->text().toFloat(),ui->showLon_LE->text().toFloat());
}
