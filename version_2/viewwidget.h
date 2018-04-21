#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

/**
* Qt include file
*/
#include <QObject>
#include <QTimer>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QDebug>
#include <QtGlobal>
#include <QTime>

/**
* user include file
*/
#include "skyDome.h"
#include "SphereSegment.h"
#include "cameracallback.h"
#include "airplanecallback.h"
#include "driftcallback.h"
#include "followadaptor.h"

#include <vector>

/**
* osg include file
*/
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osg/Node>
#include <osgOcean/OceanScene>
#include <osgOcean/FFTOceanSurface>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osg/Version>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/AnimationPathManipulator>

using namespace std;

enum SCENE_TYPE{
    SIMPLEDRIFT,
    COMPLEXDRIFT,
    DROP
};

class ViewWidget:public QWidget,public osgViewer::Viewer
{
    Q_OBJECT
public:
    explicit ViewWidget(QWidget *parent = 0);

    osg::ref_ptr<osg::Camera> createHUD();

    osg::ref_ptr<osg::Geode> createRouteLine();

    void initScene();

    void addDropScene();

    void addComplexDriftScene();

    void addSimpleDriftScene();

    osg::ref_ptr<osgOcean::FFTOceanSurface> getOceanSurface();

    void setDriftPathVec(vector<osg::Vec2> path_Vec, int updateFeq, SCENE_TYPE type);
    void setDropPathVec(vector<osg::Vec3> path_Vec);

    void addHUD(bool value);

    void addRouteLine(bool value);
private:
    osg::ref_ptr<osgOcean::OceanScene> scene;
    osg::ref_ptr<osgOcean::FFTOceanSurface> surface;
    osg::ref_ptr<osg::GraphicsContext::Traits>  traits;

    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    void setInner_type(const SCENE_TYPE &value);

    osg::ref_ptr<osg::AnimationPath> generateDriftAnimationPath(vector<osg::Vec2> path_Vec,osg::Vec3 start, int updateFrequcency, SCENE_TYPE type);
    osg::ref_ptr<osg::AnimationPath> generateDropAnimationPath(vector<osg::Vec3> path_Vec);

    void setDriftAnimationPath(vector<osg::Vec2> path_Vec, int updateFeq, SCENE_TYPE type);
    void setDropAnimationPath(osg::ref_ptr<osg::AnimationPath> path);

    osg::ref_ptr<osg::Group> root;

    vector<osg::Vec2> m_driftPath;

    SCENE_TYPE inner_type;
};

#endif // VIEWWIDGET_H
