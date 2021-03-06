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

    ui->map_View->setUrl(QUrl("qrc:/page/page/map.html"));

    ui->map_View->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * brief:a function used to generate the drift path of airpalne
 * input: it need input a vector of osg::vec3 which is the control point in the path
 * output: it will return a smooth animation path
 */

/*
 * brief:a function used to generate the drop path of airpalne
 * input: it need input a vector of osg::vec3 which is the control point in the path
 * output: it will return a smooth animation path
 */

/*
 *  brief:slot function for the airplane drop ,it's connected with the dropclick signal in sim_catalog
 */
void MainWindow::dropClickSlot()
{
    ui->main_StackWidght->setCurrentIndex(0);
    ui->dropMain_Widget->addDropScene();
}

/*
 *  brief:slot function for the drift ,it's connected with the driftclick signal in sim_catalog
 */
void MainWindow::driftClickSlot()
{
    ui->main_StackWidght->setCurrentIndex(1);
    ui->driftMain_Widget->addSimpleDriftScene();

    ui->parameter_TW->setCurrentIndex(0);
    ui->driftorData_SW->setCurrentIndex(0);
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
    QMessageBox::warning(this,QString::fromLocal8Bit("提示信息"),QString::fromLocal8Bit("切换仿真情景?\n如果切换，当前情景将丢失!"));

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
    ui->simpleData_PT->clear();

    DriftCalculateCore *driftCore = new DriftCalculateCore(SIMPLE);

    float leewayRate = ui->simpleLee_SB->text().toInt();
    driftCore->setInner_leewayRate(leewayRate * 0.01);

    int currentAng = ui->currentDirection_DIAL->value();
    float currentVel = ui->currentVelocity_LE->text().toFloat();
    MVec2 current(currentVel * cos(currentAng),currentVel * sin(currentAng));
    driftCore->setInner_currentSpeed(current);

    int windAng = ui->windDirection_DIAL->value();
    float windVel = ui->windVelocity_LE->text().toFloat();
    MVec2 wind(windVel * cos(windAng),windVel * sin(windAng));
    driftCore->setInner_windSpeed(wind);

    driftCore->setInner_initPosition(MVec2(0,0));
    driftCore->setInner_updateFrequency(1);

    int simulateTime = ui->simpleSimulateTime_LE->text().toInt();
    driftCore->setInner_simulateTime(simulateTime);

    vector<MVec2> driftVec = driftCore->getDriftRoute();

    vector<osg::Vec2> path_Vec;
    for(unsigned i = 0;i<driftVec.size();i++){
        MVec2 point = driftVec[i];
        path_Vec.push_back(osg::Vec2(point.getX(),point.getY()));
        ui->simpleData_PT->appendPlainText(QString::number(point.getX())+QString::number(point.getY()));
    }

    ui->driftMain_Widget->setDriftPathVec(path_Vec,1,SIMPLEDRIFT);
}

void MainWindow::on_dropStart_BT_clicked()
{
    int weight = ui->weight_LE->text().toInt();
    float liftDragCofficient = ui->liftDragCoefficient_LE->text().toFloat();
    float wingArea = ui->wingArea_LE->text().toFloat();
    int initPositionX = ui->initPosX_LE->text().toInt();
    int initPositionY = ui->initPosY_LE->text().toInt();
    int initPositionZ = ui->initPosZ_LE->text().toInt();
    MVec3 initPos(initPositionX,initPositionY,initPositionZ);

    int initSpeedX = ui->initSpeedX_LE->text().toInt();
    int initSpeedY = ui->initSpeedY_LE->text().toInt();
    int initSpeedZ = ui->initSpeedZ_LE->text().toInt();
    MVec3 initSpeed(initSpeedX,initSpeedY,initSpeedZ);

    DropcalculateCore *drop_core = new DropcalculateCore;
    drop_core->setInner_liftDragCofficient(liftDragCofficient);
//    drop_core->setInner_airplaneArea(allArea);
    drop_core->setInner_wingArea(wingArea);
    drop_core->setInner_flySpeed(initSpeed);
    drop_core->setInner_Position(initPos);
    drop_core->setInner_weight(weight);

    vector<MVec3> routeVec = drop_core->getDropRoute();

    vector<osg::Vec3> path_Vec;
    for(unsigned i = 0;i<routeVec.size();i++){
        MVec3 point = routeVec[i];
        path_Vec.push_back(osg::Vec3(point.getX(),point.getY(),point.getZ()));
    }

    int finalPosX = initSpeedX + routeVec.at(routeVec.size() - 1).getX();
    int finalPosY = initSpeedY + routeVec.at(routeVec.size() - 1).getY();

    ui->finalPosX_LB->setText(QString::number(finalPosX));
    ui->finalPosY_LB->setText(QString::number(finalPosY));
    ui->dropMain_Widget->setDropPathVec(path_Vec);
}

void MainWindow::on_dataPath_BT_clicked()
{
    QStringList filesList = QFileDialog::getOpenFileNames(this,"Open_Data_File",".","Data_File(*.txt)");

    int index;
    QStandardItemModel *windModel = new QStandardItemModel;
    QStandardItemModel *currentModel = new QStandardItemModel;
    for(int i = 0;i<filesList.size();i++){
        QString fileName = filesList.at(i);
        if(fileName.contains("wind")){
            index = fileName.lastIndexOf('/');
            fileName = fileName.right(fileName.length() - index -1);
            QStandardItem* nameItem = new QStandardItem(fileName);
            windModel->appendRow(nameItem);
        }
        if(fileName.contains("current")){
            index = fileName.lastIndexOf('/');
            fileName = fileName.right(fileName.length() - index -1);
            QStandardItem* nameItem = new QStandardItem(fileName);
            currentModel->appendRow(nameItem);
        }
    }

    ui->dataPath_LE->setText(filesList.at(0).left(index));
    ui->windData_TV->setModel(windModel);
    ui->currentData_TV->setModel(currentModel);
}

void MainWindow::on_complexCalculate_BT_clicked()
{
    ui->complexData_PT->clear();
    float leewayRate = ui->complexLee_SB->text().toInt();
    int driftorVecSize = ui->driftorVecSize_LE->text().toInt();
    float windDeviation = ui->windDeviation_LE->text().toFloat();
    float currentDeviation = ui->currentDeviation_LE->text().toFloat();
    int updateFeq = ui->updateFreq_LE->text().toInt();
    int simulationTime = ui->complexSimulateTime_LE->text().toInt();
    float spatialCoeffcient = ui->spatialCoeffcient_LE->text().toFloat();
    QDate beginDate = ui->begin_DE->date();
    float posLat = ui->posLat_LE->text().toFloat();
    float posLon = ui->posLon_LE->text().toFloat();
    Position initPos = calculateellipse2plane(posLat,posLon);

    DriftCalculateCore *driftCore = new DriftCalculateCore(COMPLEX);
    driftCore->setInner_leewayRate(leewayRate*0.01);
    driftCore->setInner_windDeviation(windDeviation);
    driftCore->setInner_currentDeviation(currentDeviation);
    driftCore->setInner_updateFrequency(updateFeq);
    driftCore->setInner_simulateTime(simulationTime);
    driftCore->setInner_spacialCorrelation(spatialCoeffcient);
    driftCore->setInner_beginDate(beginDate);    
    driftCore->setInner_initPosition(MVec2(initPos.x,initPos.y));
    driftCore->setInner_driftorVecSize(driftorVecSize);

    DataMap *map = new DataMap;
    if(ui->dataPath_LE->text().isEmpty()){
        QMessageBox::warning(this,QString::fromLocal8Bit("提示信息"),QString::fromLocal8Bit("请设置好数据读取路径!"));
        return;
    }
    map->setSourcePath(ui->dataPath_LE->text());

    QStringList windSourceList;
    for(int i = 0;i<ui->windData_TV->model()->rowCount();i++){
        QString sourceName = ui->windData_TV->model()->index(i,0).data().toString();
        windSourceList.append(sourceName);
    }
    map->setWindSourceFileNameList(windSourceList);

    QStringList currentSourceList;
    for(int i = 0;i<ui->currentData_TV->model()->rowCount();i++){
        QString sourceName = ui->currentData_TV->model()->index(i,0).data().toString();
        currentSourceList.append(sourceName);
    }
    map->setCurrentSourceFileNameList(currentSourceList);

    DataPipe *pipe = new DataPipe;
    pipe->setDataMap(map);

    driftCore->setDataPipe(*pipe);

    vector<MVec2> driftVec = driftCore->getDriftRoute();

    vector<osg::Vec2> path_Vec;
    for(unsigned i = 0;i<driftVec.size();i++){
        MVec2 point = driftVec[i];
        path_Vec.push_back(osg::Vec2(point.getX(),point.getY()));
        LatLon pos = Calculateplane2ellipse(point.getX(),point.getY());
        ui->complexData_PT->appendPlainText(QString::number(pos.lat)+" "+QString::number(pos.lon));
    }

    ui->driftMain_Widget->setDriftPathVec(path_Vec,updateFeq*86400,COMPLEXDRIFT);
}

//
void MainWindow::on_actionDownload_triggered()
{
      Download_widget *widget = new Download_widget(this);
      widget->show();
}

void MainWindow::on_actionAnalysis_triggered()
{
    Analysis_widget *widget = new Analysis_widget(this);
    widget->show();
}

void MainWindow::on_actionNew_triggered()
{
    Sim_catalog *simCatalog = new Sim_catalog(this);
    simCatalog->show();
    connect(simCatalog,SIGNAL(dropClick()),this,SLOT(dropClickSlot()));
    connect(simCatalog,SIGNAL(driftClick()),this,SLOT(driftClickSlot()));
}

void MainWindow::on_simpleHud_CB_stateChanged(int arg1)
{
    ui->driftMain_Widget->addHUD(arg1);
}

void MainWindow::on_complexHud_CB_stateChanged(int arg1)
{
    ui->driftMain_Widget->addHUD(arg1);
}


void MainWindow::on_simpleRoute_CB_stateChanged(int arg1)
{
    ui->driftMain_Widget->addRouteLine(arg1);
}

void MainWindow::on_complexRoute_CB_stateChanged(int arg1)
{
    ui->driftMain_Widget->addRouteLine(arg1);
}

void MainWindow::on_actionPosition_S_triggered()
{
    PosSynthesis_Widget *syn_widget = new PosSynthesis_Widget;
    syn_widget->show();
    connect(syn_widget,SIGNAL(showLatAndLon(float,float)),this,SLOT(showLatLonSlot(float,float)));
}

void MainWindow::showLatLonSlot(float lat, float lon)
{
    ui->main_StackWidght->setCurrentIndex(2);

    QWebFrame *webFrame = ui->map_View->page()->mainFrame();

    QString cmd = QString("showAddress(%1,%2)").arg(lat).arg(lon);

    webFrame->evaluateJavaScript(cmd);
}

void MainWindow::on_actionTrackBall_triggered()
{
    osgGA::TrackballManipulator* tbManipulator = new osgGA::TrackballManipulator;
    tbManipulator->setHomePosition(osg::Vec3d(0,1000,100),osg::Vec3d(0,0,0),osg::Z_AXIS);
    ui->dropMain_Widget->setManipulator(tbManipulator);
    ui->driftMain_Widget->setManipulator(tbManipulator);
}


void MainWindow::on_actionFollow_triggered()
{
    osg::ref_ptr<osg::Group> root = ui->dropMain_Widget->getSceneData()->asGroup();
    for(int i = 0;i<root->getNumChildren();i++)
    {
        osg::ref_ptr<osg::Node> node = root->getChild(i);
        if(node->getName() == "airplanemat")
        {
            node = node->asGroup()->getChild(0);
            FollowUpdater* follow= new FollowUpdater(node);
            follow->setName("follow");
            ui->dropMain_Widget->addEventHandler(follow);
        }
    }
}
