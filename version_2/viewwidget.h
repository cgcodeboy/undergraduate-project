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

class ViewWidget:public QWidget,public osgViewer::Viewer
{
    Q_OBJECT
public:
    explicit ViewWidget(QWidget *parent = 0);

    //    osg::ref_ptr<osg::Camera> createHUD();
    //    void addNode(osg::ref_ptr<osg::Node> node);
    void initScene();

    void addDropScene();

    void addComplexDriftScene();

    void addSimpleDriftScene();

    osg::ref_ptr<osgOcean::FFTOceanSurface> getOceanSurface();

    void setSimpleAnimationPath(osg::ref_ptr<osg::AnimationPath> path);

private:
    osg::ref_ptr<osgOcean::OceanScene> scene;
    osg::ref_ptr<osgOcean::FFTOceanSurface> surface;
    osg::ref_ptr<osg::GraphicsContext::Traits>  traits;

    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    osg::ref_ptr<osg::Group> root;
};

#endif // VIEWWIDGET_H
