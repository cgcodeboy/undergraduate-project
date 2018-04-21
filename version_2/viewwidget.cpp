#include "viewwidget.h"


/*
 *  brief: a function used to add a HUD to the osg scene
 */
osg::ref_ptr<osg::Camera> ViewWidget::createHUD()
{
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    camera->setViewMatrix(osg::Matrix::identity());
    camera->setRenderOrder(osg::Camera::POST_RENDER);//最后渲染
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setProjectionMatrixAsOrtho2D(0,640,0,480);
    camera->setAllowEventFocus(false);

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    osg::ref_ptr<osg::Geometry> plane = new osg::Geometry;
    osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array;
    vertex->push_back(osg::Vec3(490,330,0));
    vertex->push_back(osg::Vec3(640,330,0));
    vertex->push_back(osg::Vec3(640,480,0));
    vertex->push_back(osg::Vec3(490,480,0));
    plane->setVertexArray(vertex.get());

    osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
    color->push_back(osg::Vec4(0.5f,0.5f,0.5f,1.0f));
    plane->setColorArray(color.get());
    plane->setColorBinding(osg::Geometry::BIND_OVERALL);

    osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array;
    normal->push_back(osg::Vec3(0,0,1.0f));
    plane->setNormalArray(normal.get());
    plane->setNormalBinding(osg::Geometry::BIND_OVERALL);

    plane->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,vertex->size()));
    geode->addDrawable(plane);

    if(m_driftPath.size()!=0){
        vector<osg::Vec2> point_Vec;

        osg::ref_ptr<osg::Geometry> line = new osg::Geometry;
        osg::Vec2 start = m_driftPath.at(0);
        for(int i = 0;i<m_driftPath.size();i++){
            point_Vec.push_back(m_driftPath.at(i) - start);
        }

        int xMIN = INT_MAX,xMAX = INT_MIN,yMIN = INT_MAX,yMAX = INT_MIN;
        for(int i = 0;i<point_Vec.size();i++){
            osg::Vec2 cur = point_Vec.at(i);
            if(cur.x()<xMIN){
                xMIN = cur.x();
            }
            if(cur.x()>xMAX){
                xMAX = cur.x();
            }
            if(cur.y()<yMIN){
                yMIN = cur.y();
            }
            if(cur.y()>yMAX){
                yMAX = cur.y();
            }
        }

        float cur_width = abs(xMIN)>abs(xMAX)?abs(xMIN):abs(xMAX);
        float cur_height = abs(yMIN)>abs(yMAX)?abs(yMIN):abs(yMAX);
        float max = cur_width>cur_height?cur_width:cur_height;

        osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array;
        for(int i = 0;i<point_Vec.size();i++){
            osg::Vec2 cur = point_Vec.at(i);
            vertex->push_back(osg::Vec3(565+75*cur.x()/max,405+75*cur.y()/max,0));
        }
        line->setVertexArray(vertex.get());
        line->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,vertex->size()));

        osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
        color->push_back(osg::Vec4(0,0,0,1));
        line->setColorArray(color);
        line->setColorBinding(osg::Geometry::BIND_OVERALL);

        geode->addDrawable(line);
    }

    geode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    camera->addChild(geode.get());
    return camera;
}

osg::ref_ptr<osg::Geode> ViewWidget::createRouteLine()
{
    if(m_driftPath.size()>1){
        osg::ref_ptr<osg::Geode> line = new osg::Geode;

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

        osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array;
        osg::Vec2 start = m_driftPath.at(0);

        float height = scene->getOceanSurfaceHeight();
        for(int i = 0;i<m_driftPath.size();i++){
            osg::Vec2 cur = m_driftPath.at(i);
            vertex->push_back(osg::Vec3(cur-start,height));
        }

        geometry->setVertexArray(vertex);
        geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,vertex->size()));

        line->addDrawable(geometry);
        return line;
    }

    return NULL;
}

void ViewWidget::addRouteLine(bool value)
{
    if(value){
        osg::ref_ptr<osg::Group> root = new osg::Group;
        osg::ref_ptr<osg::Node> group = getSceneData();
        group->setName("scene");
        root->addChild(group);
        osg::ref_ptr<osg::Geode> line = createRouteLine();
        if(line){
            root->addChild(line);
        }
        setSceneData(root);
    }
    else{
        osg::ref_ptr<osg::Group> root = getSceneData()->asGroup();
        for(int i = 0;i<root->getNumChildren();i++){
            osg::ref_ptr<osg::Node> cur = root->getChild(i);
            if(cur->getName() == "scene"){
                setSceneData(cur);
            }
        }
    }
}

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

void ViewWidget::setInner_type(const SCENE_TYPE &value)
{
    inner_type = value;
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
    osg::ref_ptr<SkyDome> sky = new SkyDome(2000,16,16,cubeMap.get());
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
    scene->enableRefractions(true);
    surface->setEnvironmentMap(cubeMap.get());

    //水花
    surface->setFoamBottomHeight(2.2);
    surface->setFoamTopHeight(3.0);
    surface->enableCrestFoam(true);

    addEventHandler(surface->getEventHandler());
    addEventHandler(scene->getEventHandler());

//    setSceneData(scene);
}

/*
 * brief: a simulation scene for airplane drop from the sky
 */
void ViewWidget::addDropScene()
{
    setInner_type(DROP);
    initScene();
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
    setInner_type(COMPLEXDRIFT);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    initScene();
    osg::ref_ptr<osg::Group> driftorGroup = new osg::Group;
    driftorGroup->setName("driftor_group");
    for(int i = 1;i<=15;i++)
    {
        osg::ref_ptr<osg::MatrixTransform> driftMat = new osg::MatrixTransform;
        int x = qrand()%500,y = qrand()%500;
        driftMat->setMatrix(osg::Matrix::scale(0.005,0.005,0.005)*osg::Matrix::translate(x,y,0));
        osg::ref_ptr<osg::Node> driftPart = osgDB::readNodeFile(QString("resources/air/part_%1.obj").arg(i).toStdString());
        driftMat->addChild(driftPart);
        driftMat->setUpdateCallback(new DriftCallback(scene,"airplane"));
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
    setInner_type(SIMPLEDRIFT);
    initScene();
    osg::ref_ptr<osg::Group> driftorGroup = new osg::Group;
    driftorGroup->setName("driftor_group");
    osg::ref_ptr<osg::MatrixTransform> driftMat = new osg::MatrixTransform;
    driftMat->setMatrix(osg::Matrix::scale(0.5,0.5,0.5)*osg::Matrix::translate(0,0,0));
    osg::ref_ptr<osg::Node> driftPart = osgDB::readNodeFile("resources/driftor.3DS");
    driftMat->addChild(driftPart);
    driftMat->setUpdateCallback(new DriftCallback(scene,"driftor"));
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

void ViewWidget::setDriftPathVec(vector<osg::Vec2> path_Vec,int updateFeq,SCENE_TYPE type)
{
    m_driftPath = path_Vec;
    setDriftAnimationPath(path_Vec,updateFeq,type);
}

void ViewWidget::setDropPathVec(vector<osg::Vec3> path_Vec)
{
    osg::ref_ptr<osg::AnimationPath> path = generateDropAnimationPath(path_Vec);
    setDropAnimationPath(path);
}

void ViewWidget::addHUD(bool value)
{
    if(value){
        osg::ref_ptr<osg::Group> root = new osg::Group;
        osg::ref_ptr<osg::Node> group = getSceneData();
        group->setName("scene");
        root->addChild(group);
        osg::ref_ptr<osg::Camera> cameraHUD = createHUD();
        cameraHUD->setName("HUD");
        root->addChild(cameraHUD);
        setSceneData(root);
    }
    else{
        osg::ref_ptr<osg::Group> root = getSceneData()->asGroup();
        for(int i = 0;i<root->getNumChildren();i++){
            osg::ref_ptr<osg::Node> cur = root->getChild(i);
            if(cur->getName() == "scene"){
                setSceneData(cur);
            }
        }
    }
}

void ViewWidget::setDriftAnimationPath(vector<osg::Vec2> path_Vec,int updateFeq,SCENE_TYPE type)
{
    osg::ref_ptr<osg::Group> root = scene->asGroup();
    for(int i = 0;i<root->getNumChildren();i++)
    {
        if(root->getChild(i)->getName() == "driftor_group")
        {
            osg::ref_ptr<osg::Group> drift_Group = root->getChild(i)->asGroup();

            for(int j = 0;j<drift_Group->getNumChildren();j++){
                osg::ref_ptr<osg::Node> cur = drift_Group->getChild(j);

                osg::ref_ptr<osg::AnimationPathCallback> animationPathCallback = new osg::AnimationPathCallback;

                osg::MatrixTransform* matrixTran = dynamic_cast<osg::MatrixTransform*>(cur->asTransform());
                if(matrixTran){
                    osg::Matrix  matrix = matrixTran->getMatrix();//osg::computeLocalToWorld(nv.getNodePath());
                    osg::Vec3d pos = matrix.getTrans();

                    osg::ref_ptr<osg::AnimationPath> path = generateDriftAnimationPath(path_Vec,pos,updateFeq,type);

                    animationPathCallback->setAnimationPath(path);

                    matrixTran->setUpdateCallback(animationPathCallback);
                }
            }
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
            osg::ref_ptr<osg::AnimationPathCallback> callback = new osg::AnimationPathCallback;
            callback->setAnimationPath(path);

            path->setLoopMode(osg::AnimationPath::NO_LOOPING);
            osg::ref_ptr<osg::Node> airPlaneMat = root->getChild(i);
            airPlaneMat->setUpdateCallback(callback);
            //            osgGA::TrackballManipulator *tb = dynamic_cast<osgGA::TrackballManipulator*>( this->getCameraManipulator());
            //            tb->setHomePosition(osg::Vec3(5000,5000,50),osg::Vec3(4500,4500,0),osg::Z_AXIS);
            //            setCameraManipulator(tb);
        }
    }
}

osg::ref_ptr<osg::AnimationPath> ViewWidget::generateDriftAnimationPath(vector<osg::Vec2> path_Vec,osg::Vec3 start,int updateFrequcency,SCENE_TYPE type)
{
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;

    path->setLoopMode(osg::AnimationPath::LOOP);

    osg::Vec2 initPos = path_Vec.at(0);

    for(int i = 0;i<path_Vec.size();i++)
    {
        osg::Vec2 cur = path_Vec[i];

        osg::Vec3f pos(cur-initPos,0);
        osg::AnimationPath::ControlPoint *point = new osg::AnimationPath::ControlPoint;
        if(type == SIMPLEDRIFT){
            point->setScale(osg::Vec3(0.5,0.5,0.5));
        }else{
            point->setScale(osg::Vec3(0.005,0.005,0.005));
        }
        point->setPosition(start+pos);
        path->insert(i*updateFrequcency,*point);
        delete point;
    }

    return path;
}

osg::ref_ptr<osg::AnimationPath> ViewWidget::generateDropAnimationPath(vector<osg::Vec3> path_Vec)
{
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;

    for(int i = 0;i<path_Vec.size();i++)
    {
        osg::Vec3 cur = path_Vec[i];

        osg::AnimationPath::ControlPoint *point = new osg::AnimationPath::ControlPoint;
        point->setPosition(osg::Vec3f(cur.x(),cur.y(),cur.z()));
        point->setScale(osg::Vec3f(0.005,0.005,0.005));
        path->insert(i,*point);
        delete point;
    }

    return path;
}

