#-------------------------------------------------
#
# Project created by QtCreator 2018-01-03T09:41:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = version_1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sim_catalog.cpp

HEADERS  += mainwindow.h \
    sim_catalog.h

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

