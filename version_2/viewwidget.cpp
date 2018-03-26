#include "viewwidget.h"


/*
 *  brief: a function used to add a HUD to the osg scene
 */
//osg::ref_ptr<osg::Camera> ViewWidget::createHUD()
//{
//    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
//    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
//    camera->setViewMatrix(osg::Matrix::identity());
//    camera->setRenderOrder(osg::Camera::POST_RENDER);//最后渲染
//    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//    camera->setProjectionMatrixAsOrtho2D(0,640,0,480);
//    camera->setAllowEventFocus(false);

//    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

//    osg::ref_ptr<osg::Geometry> plane = new osg::Geometry;
//    osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array;
//    vertex->push_back(osg::Vec3(540,400,0));
//    vertex->push_back(osg::Vec3(640,400,0));
//    vertex->push_back(osg::Vec3(640,480,0));
//    vertex->push_back(osg::Vec3(540,480,0));
//    plane->setVertexArray(vertex.get());
//    plane->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,vertex->size()));
//    geode->addDrawable(plane);

//    geode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
//    geode->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::OFF);
//    camera->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
//    camera->addChild(geode.get());
//    return camera;
//}

/*
 *  brief: the construct function
 *          in this function, we need to initalize the context traits, then use these traits to
 *          to initalize the GraphicsWindowQt, at last use this window to set the camera context.
 */
ViewWidget::ViewWidget(QWidget *parent):QWidget(parent)
{
//    root = new osg::Group;
//    //创建图形上下文特性
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    traits = new osg::GraphicsContext::Traits;
    traits->x = 0;
    traits->y = 0;
    traits->width = 640;
    traits->height = 480;
    traits->windowDecoration = false;
    traits->doubleBuffer = true;
    traits->sharedContext =0;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();
    setThreadingModel(osgViewer::Viewer::SingleThreaded);

    //设置图形上下文
    osgQt::GraphicsWindowQt *gw = new osgQt::GraphicsWindowQt(traits.get());
    getCamera()->setGraphicsContext(gw);

    getCamera()->setProjectionMatrixAsPerspective(40.0,static_cast<double>(traits->width)/static_cast<double>(traits->height),0.1f,1000.0f);
    getCamera()->setViewport(new osg::Viewport(0,0,traits->width,traits->height));

    osg::ref_ptr<osgGA::TrackballManipulator> tb = new osgGA::TrackballManipulator;
    tb->setHomePosition(osg::Vec3d(0,1000,100),osg::Vec3d(0,0,0),osg::Z_AXIS);
    setCameraManipulator(tb.get());

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(gw->getGLWidget(),0,0);
    setLayout(layout);
}

/*
 * brief: the update function to repaint the scene
 */
void ViewWidget::paintEvent(QPaintEvent *event)
{
    frame();
    update();
}

/*
 * brief: the window resize function
 */
void ViewWidget::resizeEvent(QResizeEvent *event)
{
    this->traits->width = event->size().width();
    this->traits->height = event->size().height();
    getCamera()->setProjectionMatrixAsPerspective(40.0,static_cast<double>(traits->width)/static_cast<double>(traits->height),0.1f,1000.0f);
    getCamera()->setViewport(new osg::Viewport(0,0,traits->width,traits->height));
}

/*
 * brief: the initialize function
 *          in this function, we will initialize the ocean scene, the skybox
 */
void ViewWidget::initScene()
{
    root = new osg::Group;

    surface = new osgOcean::FFTOceanSurface(64,256,17,osg::Vec2(1.1f,1.1f),12,10,0.8,1e-8,true,2.5,20.0,256);
    surface->enableEndlessOcean(true);
    surface->setIsChoppy(true);
    scene = new osgOcean::OceanScene(surface.get());

    osg::ref_ptr<osg::TextureCubeMap> cubeMap = new osg::TextureCubeMap;
    cubeMap->setInternalFormat(GL_RGB);
    cubeMap->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
    cubeMap->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
    cubeMap->setWrap(osg::Texture::WRAP_S,osg::Texture::CLAMP_TO_EDGE);
    cubeMap->setWrap(osg::Texture::WRAP_T,osg::Texture::CLAMP_TO_EDGE);
    cubeMap->setImage(osg::TextureCubeMap::NEGATIVE_X,osgDB::readImageFile("resources/textures/sky_clear/west.png"));
    cubeMap->setImage(osg::TextureCubeMap::POSITIVE_X,osgDB::readImageFile("resources/textures/sky_clear/east.png"));
    cubeMap->setImage(osg::TextureCubeMap::NEGATIVE_Y,osgDB::readImageFile("resources/textures/sky_clear/up.png"));
    cubeMap->setImage(osg::TextureCubeMap::POSITIVE_Y,osgDB::readImageFile("resources/textures/sky_clear/down.png"));
    cubeMap->setImage(osg::TextureCubeMap::NEGATIVE_Z,osgDB::readImageFile("resources/textures/sky_clear/south.png"));
    cubeMap->setImage(osg::TextureCubeMap::POSITIVE_Z,osgDB::readImageFile("resources/textures/sky_clear/north.png"));
    //天空盒
    osg::ref_ptr<SkyDome> sky = new SkyDome(1900,16,16,cubeMap.get());
//    sky->setNodeMask(scene->getReflectedSceneMask() | scene->getNormalSceneMask());
    osg::MatrixTransform * mat =new osg::MatrixTransform;
    mat->setDataVariance(osg::Object::DYNAMIC);
    mat->setMatrix(osg::Matrixf::translate(osg::Vec3d(0,0,0)));
    mat->addChild(sky.get());
    mat->setCullCallback(new CameraTrackCallback);
    scene->addChild(mat);

    //雾效和反射
    scene->setAboveWaterFog(0.0012,osg::Vec4(0.67,0.87,0.97,1.0));
    scene->enableReflections(true);
    surface->setEnvironmentMap(cubeMap.get());

    //水花
    surface->setFoamBottomHeight(2.2);
    surface->setFoamTopHeight(3.0);
    surface->enableCrestFoam(true);

    addEventHandler(surface->getEventHandler());
    addEventHandler(scene->getEventHandler());

    setSceneData(scene);

    //    root->addChild(createHUD());
}

//void ViewWidget::addNode(osg::ref_ptr<osg::Node> node)
//{
//    root->addChild(node.get());
//}

/*
 * brief: a simulation scene for airplane drop from the sky
 */
void ViewWidget::addDropScene()
{
    osg::ref_ptr<osg::Node> airplane = osgDB::readNodeFile("resources/tian.obj");
    osg::ref_ptr<osg::MatrixTransform> airplaneMat = new osg::MatrixTransform;
    airplaneMat->setName("airplanemat");
    airplaneMat->setMatrix(osg::Matrix::scale(0.005,0.005,0.005)*osg::Matrix::translate(0,0,100));
    airplaneMat->setUpdateCallback(new AirplaneCallback(scene));
    airplaneMat->addChild(airplane);
    scene->addChild(airplaneMat);
    setSceneData(scene);
    update();
}

/*
 * brief: a simulation scene for all the driftor drift in the ocean
 */
void ViewWidget::addComplexDriftScene()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    osg::ref_ptr<osg::Group> root = scene->asGroup();
    for(int i = 0;i<root->getNumChildren();i++)
    {
        if(root->getChild(i)->getName() == "driftor_group")
        {
            root->removeChild(i);
        }
    }
    osg::ref_ptr<osg::Group> driftorGroup = new osg::Group;
    driftorGroup->setName("driftor_group");
    for(int i = 0;i<15;i++)
    {
        osg::ref_ptr<osg::MatrixTransform> driftMat = new osg::MatrixTransform;
        int x = qrand()%500,y = qrand()%500;
        driftMat->setMatrix(osg::Matrix::scale(0.01,0.01,0.01)*osg::Matrix::translate(x,y,0));
        osg::ref_ptr<osg::Node> driftPart = osgDB::readNodeFile(QString("resources/air/part_%1.obj").arg(i).toStdString());
        driftMat->addChild(driftPart);
        driftMat->setUpdateCallback(new DriftCallback(scene));
        driftorGroup->addChild(driftMat);
    }
    scene->addChild(driftorGroup);
    setSceneData(scene);
    update();
}

/*
 * brief: a simulation scene for a simple driftor drifts in the ocean
 */
void ViewWidget::addSimpleDriftScene()
{
    osg::ref_ptr<osg::Group> root = scene->asGroup();
    for(int i = 0;i<root->getNumChildren();i++)
    {
        if(root->getChild(i)->getName() == "driftor_group")
        {
            root->removeChild(i);
        }
    }
    osg::ref_ptr<osg::Group> driftorGroup = new osg::Group;
    driftorGroup->setName("driftor_group");
    osg::ref_ptr<osg::MatrixTransform> driftMat = new osg::MatrixTransform;
    driftMat->setMatrix(osg::Matrix::scale(0.01,0.01,0.01)*osg::Matrix::translate(0,0,0));
    osg::ref_ptr<osg::Node> driftPart = osgDB::readNodeFile("resources/air/part_1.obj");
    driftMat->addChild(driftPart);
    driftMat->setUpdateCallback(new DriftCallback(scene));
    driftorGroup->addChild(driftMat);
    scene->addChild(driftorGroup);
    setSceneData(scene);
    update();
}

/*
 * brief: return the ocean surface;
 */
osg::ref_ptr<osgOcean::FFTOceanSurface> ViewWidget::getOceanSurface()
{
    return this->surface;
}

void ViewWidget::setSimpleDriftAnimationPath(osg::ref_ptr<osg::AnimationPath> path)
{
    osg::ref_ptr<osg::Group> root = scene->asGroup();
    for(int i = 0;i<root->getNumChildren();i++)
    {
        if(root->getChild(i)->getName() == "driftor_group")
        {
            qDebug()<<"haha";
            osg::ref_ptr<osg::Group> drift_Group = root->getChild(i)->asGroup();

            path->setLoopMode(osg::AnimationPath::NO_LOOPING);
            osg::ref_ptr<osg::AnimationPathCallback> animationPathCallback = new osg::AnimationPathCallback;
            animationPathCallback->setAnimationPath(path);

            osg::ref_ptr<osg::Node> matTrans = drift_Group->getChild(0);
            matTrans->setUpdateCallback(animationPathCallback);
        }
    }
}

void ViewWidget::setDropAnimationPath(osg::ref_ptr<osg::AnimationPath> path)
{
    osg::ref_ptr<osg::Group> root = scene->asGroup();
    for(int i = 0;i<root->getNumChildren();i++)
    {
        if(root->getChild(i)->getName() == "airplanemat")
        {
            qDebug()<<"haha";
            osg::ref_ptr<osg::AnimationPathCallback> callback = new osg::AnimationPathCallback;
            callback->setAnimationPath(path);

            path->setLoopMode(osg::AnimationPath::NO_LOOPING);
            osg::ref_ptr<osg::Node> airPlaneMat = root->getChild(i);
            airPlaneMat->setUpdateCallback(callback);
            osgGA::TrackballManipulator *tb = dynamic_cast<osgGA::TrackballManipulator*>( this->getCameraManipulator());
            tb->setHomePosition(osg::Vec3(5000,5000,50),osg::Vec3(4500,4500,0),osg::Z_AXIS);
            setCameraManipulator(tb);
        }
    }
}

