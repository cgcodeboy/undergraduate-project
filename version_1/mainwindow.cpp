#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置相关属性


    Sim_catalog *simCatalog = new Sim_catalog(this);
    simCatalog->show();
    connect(simCatalog,SIGNAL(dropClick()),this,SLOT(dropClickSlot()));
    connect(simCatalog,SIGNAL(driftClick()),this,SLOT(driftClickSlot()));
    connect(simCatalog,SIGNAL(routeClick()),this,SLOT(routeClickSlot()));

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
    ui->dropMain_Widget->initScene();
    ui->dropMain_Widget->addDropScene();
}

/*
 *  brief:slot function for the drift ,it's connected with the driftclick signal in sim_catalog
 */
void MainWindow::driftClickSlot()
{
    ui->main_StackWidght->setCurrentIndex(1);
    ui->driftMain_Widget->initScene();
    ui->driftMain_Widget->addSimpleDriftScene();

    ui->parameter_TW->setCurrentIndex(0);
    ui->driftorData_SW->setCurrentIndex(0);
}

/*
 *  brief:slot function for the route ,it's connected with the routeclick signal in sim_catalog
 */
void MainWindow::routeClickSlot()
{
    ui->main_StackWidght->setCurrentIndex(2);

    ui->map_View->setUrl(QUrl("qrc:/page/page/map.html"));

    ui->map_View->show();
}

void MainWindow::on_subject_TW_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->text(column) == "海浪高度"){

    }
}

/*
 *  brief: if click the simple simulation tab, it will display the simple data stack widget,
 *          else it will display the all data stack widget
 */
void MainWindow::on_parameter_TW_currentChanged(int index)
{

}

/*
 *  brief: a slot function for the wind direction dial, it will change the LCD number depended on the value
 *          of the dial, and it will also chage the wind direction label  depended on the value of the dial
 */
void MainWindow::on_windDirection_DIAL_valueChanged(int value)
{
    if(value>0&&value<90){
        ui->windDirection_LB->setText(QString::fromLocal8Bit("西偏南"));
        ui->windDirection_LCD->display(90-value);
    }
    else if(value>90&&value<180){
        ui->windDirection_LB->setText(QString::fromLocal8Bit("西偏北"));
        ui->windDirection_LCD->display(value-90);
    }
    else if(value>180&&value<270){
        ui->windDirection_LB->setText(QString::fromLocal8Bit("东偏北"));
        ui->windDirection_LCD->display(270-value);
    }
    else if(value>270&&value<360){
        ui->windDirection_LB->setText(QString::fromLocal8Bit("东偏南"));
        ui->windDirection_LCD->display(value-270);
    }
    else if(value == 0){
        ui->windDirection_LB->setText(QString::fromLocal8Bit("正南"));
        ui->windDirection_LCD->display(0);
    }
    else if(value == 90){
        ui->windDirection_LB->setText(QString::fromLocal8Bit("正西"));
        ui->windDirection_LCD->display(0);
    }
    else if(value == 180){
        ui->windDirection_LB->setText(QString::fromLocal8Bit("正北"));
        ui->windDirection_LCD->display(0);
    }
    else if(value == 270){
        ui->windDirection_LB->setText(QString::fromLocal8Bit("正东"));
        ui->windDirection_LCD->display(0);
    }
}

/*
 *  brief: a slot function for the current direction dial, it will change the LCD number depended on the value
 *          of the dial, and it will also chage the current direction label  depended on the value of the dial
 */
void MainWindow::on_currentDirection_DIAL_valueChanged(int value)
{
    if(value>0&&value<90){
        ui->currentDirection_LB->setText(QString::fromLocal8Bit("西偏南"));
        ui->currentDirection_LCD->display(90-value);
    }
    else if(value>90&&value<180){
        ui->currentDirection_LB->setText(QString::fromLocal8Bit("西偏北"));
        ui->currentDirection_LCD->display(value-90);
    }
    else if(value>180&&value<270){
        ui->currentDirection_LB->setText(QString::fromLocal8Bit("东偏北"));
        ui->currentDirection_LCD->display(270-value);
    }
    else if(value>270&&value<360){
        ui->currentDirection_LB->setText(QString::fromLocal8Bit("东偏南"));
        ui->currentDirection_LCD->display(value-270);
    }
    else if(value == 0){
        ui->currentDirection_LB->setText(QString::fromLocal8Bit("正南"));
        ui->currentDirection_LCD->display(0);
    }
    else if(value == 90){
        ui->currentDirection_LB->setText(QString::fromLocal8Bit("正西"));
        ui->currentDirection_LCD->display(0);
    }
    else if(value == 180){
        ui->currentDirection_LB->setText(QString::fromLocal8Bit("正北"));
        ui->currentDirection_LCD->display(0);
    }
    else if(value == 270){
        ui->currentDirection_LB->setText(QString::fromLocal8Bit("正东"));
        ui->currentDirection_LCD->display(0);
    }
}

void MainWindow::on_parameter_TW_tabBarClicked(int index)
{
    QMessageBox box(QMessageBox::Warning,QString::fromLocal8Bit("提示信息"),\
                                                               QString::fromLocal8Bit("切换仿真情景?\n如果切换，当前情景将丢失!"));

    box.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);

    box.show();

    if(box.exec() == QMessageBox::Ok)
    {
        qDebug()<<"yes";
        ui->driftorData_SW->setCurrentIndex(index);
        if(index == 0)
        {
            ui->driftMain_Widget->addSimpleDriftScene();
        }
        else
        {
            ui->driftMain_Widget->addComplexDriftScene();
        }
    }
    else
    {
        if(ui->parameter_TW->currentIndex() == 0)
        {
            ui->parameter_TW->setCurrentIndex(0);
        }
        else
        {
            ui->parameter_TW->setCurrentIndex(1);
            qDebug()<<ui->parameter_TW->currentIndex();
        }
    }
}

/*
 * brief: the slot function for the calculate button
 *          if the button is clicked, the ocean surface must
 *          update immediately, and the change of drift condition
 *          will bring to the change of drift route, so the route
 *          must be calculate again, and the driftor will drift alone
 *          the new route.
 */
void MainWindow::on_calculate_PB_clicked()
{
    //update the ocean surface
    float windVelocity = ui->windVelocity_LE->text().toFloat();
    ui->driftMain_Widget->getOceanSurface()->setWindSpeed(windVelocity);

    int degree = ui->windDirection_DIAL->value();
    qDebug()<<cos(qDegreesToRadians(static_cast<float>(270-degree)))<<" "<<sin(qDegreesToRadians(static_cast<float>(270-degree)));
    ui->driftMain_Widget->getOceanSurface()->setWindDirection(\
                osg::Vec2f(cos(qDegreesToRadians(static_cast<float>(270-degree))),\
                           sin(qDegreesToRadians(static_cast<float>(270-degree)))));
}
