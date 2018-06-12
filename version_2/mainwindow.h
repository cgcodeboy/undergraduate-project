#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
 * Qt header file
 */
#include <QMainWindow>
#include <QDebug>
#include <QTreeWidget>
#include <QtWebKit>
#include <QMessageBox>
#include <QtMath>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>

/*
 * user header file
 */
#include <sim_catalog.h>
#include <download_widget.h>
#include <driftcalculatecore.h>
#include <dropcalculatecore.h>
#include <followadaptor.h>
#include <analysis_widget.h>
#include <possynthesis_widget.h>
#include <viewwidget.h>

/*
 * osg header file
 */
#include <osg/AnimationPath>
#include <osgGA/TrackballManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/FlightManipulator>
#include <osg/Vec2>
#include <osg/Vec3>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

//    osg::ref_ptr<osg::AnimationPath> generateDriftAnimationPath(vector<osg::Vec2> path_Vec, int updateFrequcency, SCENE_TYPE type);
//    osg::ref_ptr<osg::AnimationPath> generateDropAnimationPath(vector<osg::Vec3> path_Vec);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

//    osg::ref_ptr<osg::AnimationPath> generateAnimationPath(osg::ref_ptr<osg::Vec2Array>);


private slots:
    void dropClickSlot();
    void driftClickSlot();
    void on_subject_TW_itemClicked(QTreeWidgetItem *item, int column);
    void on_parameter_TW_currentChanged(int index);
    void on_windDirection_DIAL_valueChanged(int value);
    void on_currentDirection_DIAL_valueChanged(int value);
    void on_calculate_PB_clicked();
    void on_dropStart_BT_clicked();
    void on_dataPath_BT_clicked();
    void on_complexCalculate_BT_clicked();
    void on_actionDownload_triggered();
    void on_actionAnalysis_triggered();
    void on_actionNew_triggered();
    void on_simpleHud_CB_stateChanged(int arg1);
    void on_complexHud_CB_stateChanged(int arg1);
    void on_simpleRoute_CB_stateChanged(int arg1);
    void on_complexRoute_CB_stateChanged(int arg1);
    void on_actionPosition_S_triggered();

    void showLatLonSlot(float lat,float lon);
    void on_actionTrackBall_triggered();
    void on_actionFollow_triggered();
};

#endif // MAINWINDOW_H
