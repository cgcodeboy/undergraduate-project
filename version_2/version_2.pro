#-------------------------------------------------
#
# Project created by QtCreator 2018-01-03T09:41:42
#
#-------------------------------------------------

QT       += core gui opengl
QT += webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = version_2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sim_catalog.cpp \
    skydome.cpp \
    spheresegment.cpp \
    viewwidget.cpp \
    driftcalculatecore.cpp \
    dropcalculatecore.cpp \
    driftor.cpp \
    datapipe.cpp \
    datamap.cpp

HEADERS  += mainwindow.h \
    sim_catalog.h \
    cameracallback.h \
    skyDome.h \
    SphereSegment.h \
    viewwidget.h \
    airplanecallback.h \
    driftcallback.h \
    driftcalculatecore.h \
    all_include.h \
    dropcalculatecore.h \
    driftor.h \
    datapipe.h \
    datamap.h

FORMS    += mainwindow.ui \
    sim_catalog.ui

RESOURCES += \
    picture.qrc

CONFIG+=debug_and_release
INCLUDEPATH += D:\osg\OpenSceneGraph-3.2.1-rc3\include
CONFIG(debug, debug|release) {
    LIBS+=-LD:\osg\OpenSceneGraph-3.2.1-rc3\lib -lOpenThreadsd -losgd -losgDBd -losgUtild -losgGAd -losgViewerd -losgTextd -losgOceanD -losgQtd
} else {
    LIBS+=-LD:\osg\OpenSceneGraph-3.2.1-rc3\lib -lOpenThreads -losg -losgDB -losgUtil -losgGA -losgViewer -losgText -losgOcean -losgQt
}

DISTFILES += \
    version_2.pro.user

