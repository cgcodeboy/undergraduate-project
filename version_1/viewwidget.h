﻿#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QObject>
#include <QTimer>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QDebug>

#include "skyDome.h"
#include "SphereSegment.h"

#include "cameracallback.h"


#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osg/Node>
#include <osgOcean/OceanScene>
#include <osgOcean/FFTOceanSurface>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

class ViewWidget:public QWidget,public osgViewer::Viewer
{
    Q_OBJECT
public:
    explicit ViewWidget(QWidget *parent = 0);

//    osg::ref_ptr<osg::Camera> createHUD();
//    void addNode(osg::ref_ptr<osg::Node> node);

private:
    osg::ref_ptr<osgOcean::OceanScene> scene;
//    osg::ref_ptr<osgOcean::FFTOceanSurface> surface;
    osg::ref_ptr<osg::GraphicsContext::Traits>  traits;

    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    void initScene();

    osg::ref_ptr<osg::Group> root;
};

#endif // VIEWWIDGET_H
