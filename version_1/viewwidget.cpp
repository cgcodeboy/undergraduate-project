#include "viewwidget.h"

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

    initScene();
    update();
}

void ViewWidget::paintEvent(QPaintEvent *event)
{
    frame();
    update();
}

void ViewWidget::resizeEvent(QResizeEvent *event)
{
    this->traits->width = event->size().width();
    this->traits->height = event->size().height();
    getCamera()->setProjectionMatrixAsPerspective(40.0,static_cast<double>(traits->width)/static_cast<double>(traits->height),0.1f,1000.0f);
    getCamera()->setViewport(new osg::Viewport(0,0,traits->width,traits->height));
}

void ViewWidget::initScene()
{
    root = new osg::Group;

    osg::ref_ptr<osgOcean::FFTOceanSurface> surface = new osgOcean::FFTOceanSurface(64,256,17,osg::Vec2(1.1f,1.1f),12,10,0.8,1e-8,true,2.5,20.0,256);
    surface->enableEndlessOcean(true);
    surface->setIsChoppy(true);
    osg::ref_ptr<osgOcean::OceanScene> scene = new osgOcean::OceanScene(surface.get());

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

    osg::ref_ptr<osg::Node> airplane = osgDB::readNodeFile("resources/tian.obj");
    osg::ref_ptr<osg::MatrixTransform> airplaneMat = new osg::MatrixTransform;
    airplaneMat->setMatrix(osg::Matrix::scale(0.005,0.005,0.005)*osg::Matrix::translate(0,0,100));
    airplaneMat->setUpdateCallback(new AirplaneCallback(scene));
    airplaneMat->addChild(airplane);
    scene->addChild(airplaneMat);

    //    root->addChild(createHUD());
    setSceneData(scene.get());
}

//void ViewWidget::addNode(osg::ref_ptr<osg::Node> node)
//{
//    root->addChild(node.get());
//}

