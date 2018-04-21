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
    if(ui->driftX_LE->text().isEmpty()||ui->driftY_LE->text().isEmpty()){
        QMessageBox::warning(this,"warn information","please input the drift information");
        return;
    }
    float before_Lat = ui->beforeLat_LE->text().toFloat();
    float before_Lon = ui->beforeLon_LE->text().toFloat();

    float dropX = ui->dropX_LE->text().toFloat();
    float dropY = ui->dropY_LE->text().toFloat();

    float driftX = ui->driftX_LE->text().toFloat();
    float driftY = ui->driftY_LE->text().toFloat();

    Position beforePos = calculateellipse2plane(before_Lat,before_Lon);
    float x = beforePos.x + dropX + driftX;
    float y = beforePos.y + dropY + driftY;

    LatLon afterPos = Calculateplane2ellipse(x,y);

    ui->afterLat_LE->setText(QString::number(afterPos.lat));
    ui->afterLon_LE->setText(QString::number(afterPos.lon));
}

void PosSynthesis_Widget::on_mapShow_BT_clicked()
{
    if(ui->afterLat_LE->text().isEmpty()||ui->afterLon_LE->text().isEmpty()){
        QMessageBox::warning(this,"warn information","please input lat and lon information need to show in map");
        return;
    }
    emit showLatAndLon(ui->afterLat_LE->text().toFloat(),ui->afterLon_LE->text().toFloat());
}
