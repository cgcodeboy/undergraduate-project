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

osg::ref_ptr<osg::AnimationPath> MainWindow::generateDriftAnimationPath(vector<osg::Vec2> path_Vec)
{
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;

    for(int i = 0;i<path_Vec.size();i++)
    {
        osg::Vec2 cur = path_Vec[i];

        osg::AnimationPath::ControlPoint *point = new osg::AnimationPath::ControlPoint;
        point->setPosition(osg::Vec3f(cur.x(),cur.y(),0));
        point->setScale(osg::Vec3f(0.005,0.005,0.005));
        path->insert(i,*point);
    }

    return path;
}

osg::ref_ptr<osg::AnimationPath> MainWindow::generateDropAnimationPath(vector<osg::Vec3> path_Vec)
{
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;

    for(int i = 0;i<path_Vec.size();i++)
    {
        osg::Vec3 cur = path_Vec[i];

        osg::AnimationPath::ControlPoint *point = new osg::AnimationPath::ControlPoint;
        point->setPosition(osg::Vec3f(cur.x(),cur.y(),cur.z()));
        //point->setRotation(osg::Quat(-90,osg::Z_AXIS));
        point->setScale(osg::Vec3f(0.005,0.005,0.005));
        path->insert(i,*point);
    }

    return path;
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

    DriftCalculateCore *driftCore = new DriftCalculateCore(SIMPLE);

    driftCore->setInner_initPosition(MVec2(0,0));
    driftCore->setInner_updateFrequency(1);
    driftCore->setInner_simulateTime(24);
    driftCore->setInner_leewayRate(0.2);
    driftCore->setInner_currentSpeed(MVec2(8,2));
    driftCore->setInner_windSpeed(MVec2(-5,7));


    vector<MVec2> driftVec = driftCore->getDriftRoute();
    for(unsigned i = 0;i<driftVec.size();i++){
        MVec2 point = driftVec[i];
        ui->simpleData_PT->appendPlainText(QString::number(point.getX())+QString::number(point.getY()));
    }

    vector<osg::Vec2> path_Vec;
    for(unsigned i = 0;i<driftVec.size();i++){
        MVec2 point = driftVec[i];
        path_Vec.push_back(osg::Vec2(point.getX(),point.getY()));
    }

    osg::ref_ptr<osg::AnimationPath> driftPath = generateDriftAnimationPath(path_Vec);
    ui->driftMain_Widget->setSimpleDriftAnimationPath(driftPath);
}

void MainWindow::on_dropStart_BT_clicked()
{
    DropcalculateCore *drop_core = new DropcalculateCore;
    drop_core->setInner_airplaneArea(100);
    drop_core->setInner_wingArea(10);
    drop_core->setInner_flySpeed(MVec3(100,100,0));
    drop_core->setInner_Position(MVec3(0,0,1000));
    drop_core->setInner_weight(20000);

    vector<MVec3> routeVec = drop_core->getDropRoute();

    vector<osg::Vec3> path_Vec;
    for(unsigned i = 0;i<routeVec.size();i++){
        MVec3 point = routeVec[i];
        path_Vec.push_back(osg::Vec3(point.getX(),point.getY(),point.getZ()));
    }

    osg::ref_ptr<osg::AnimationPath> dropPath = generateDropAnimationPath(path_Vec);
    ui->dropMain_Widget->setDropAnimationPath(dropPath);

//    osg::ref_ptr<osgGA::AnimationPathManipulator> apm = new osgGA::AnimationPathManipulator;
//    apm->setAnimationPath(dropPath);
//    ui->dropMain_Widget->setCameraManipulator(apm);
}

void MainWindow::on_VR_BT_clicked()
{
    osgGA::NodeTrackerManipulator *tm = new osgGA::NodeTrackerManipulator;
    osgGA::NodeTrackerManipulator::TrackerMode trackerMode = osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION;
    osgGA::NodeTrackerManipulator::RotationMode rotationMode = osgGA::NodeTrackerManipulator::TRACKBALL;

    tm->setTrackerMode(trackerMode);
    tm->setRotationMode(rotationMode);

    osg::ref_ptr<osg::Group> root = ui->dropMain_Widget->getSceneData()->asGroup();
    for(int i = 0;i<root->getNumChildren();i++)
    {
        osg::ref_ptr<osg::Node> node = root->getChild(i);
        if(node->getName() == "airplanemat")
        {
            node = node->asGroup()->getChild(0);
            ui->dropMain_Widget->addEventHandler(new FollowUpdater(node));
//            osg::BoundingSphere sphere = node->getBound();
//            qDebug()<<"Radius"<<sphere.radius();
//            tm->setTrackNode(node);
//            tm->setDistance(sphere.radius());
//            ui->dropMain_Widget->setCameraManipulator(tm);
        }
    }
}
