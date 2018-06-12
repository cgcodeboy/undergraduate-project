#-------------------------------------------------
#
# Project created by QtCreator 2018-01-03T09:41:42
#
#-------------------------------------------------

QT       += core gui opengl
QT += webkit webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlightSilulation
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
    datamap.cpp \
    download_widget.cpp \
    analysis_widget.cpp \
    all_include.cpp \
    httpdownload.cpp \
    possynthesis_widget.cpp

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
    datamap.h \
    followadaptor.h \
    download_widget.h \
    analysis_widget.h \
    httpdownload.h \
    possynthesis_widget.h

FORMS    += mainwindow.ui \
    sim_catalog.ui \
    download_widget.ui \
    analysis_widget.ui \
    possynthesis_widget.ui

RESOURCES += \
    picture.qrc

RC_FILE += icon.rc

CONFIG+=debug_and_release
INCLUDEPATH += D:\osg\OpenSceneGraph-3.2.1-rc3\include $$quote(D:\Program Files (x86)\netCDF 4.6.1\include) D:\boost\boost_1_66_0
CONFIG(debug, debug|release) {
    LIBS+=-LD:\osg\OpenSceneGraph-3.2.1-rc3\lib -lOpenThreadsd -losgd -losgDBd -losgUtild -losgGAd -losgViewerd -losgTextd -losgOceanD -losgQtd
} else {
    LIBS+=-LD:\osg\OpenSceneGraph-3.2.1-rc3\lib -lOpenThreads -losg -losgDB -losgUtil -losgGA -losgViewer -losgText -losgOcean -losgQt
    LIBS+=-L$$quote(D:\Program Files (x86)\netCDF 4.6.1\lib) -lnetcdf
}

DISTFILES += \
    version_2.pro.user \
    icon.rc

