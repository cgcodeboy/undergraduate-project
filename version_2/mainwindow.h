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

/*
 * user header file
 */
#include <sim_catalog.h>
#include <driftcalculatecore.h>
#include <dropcalculatecore.h>

/*
 * osg header file
 */
#include <osg/AnimationPath>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/AnimationPathManipulator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    osg::ref_ptr<osg::AnimationPath> generateDriftAnimationPath(vector<osg::Vec2> path_Vec);
    osg::ref_ptr<osg::AnimationPath> generateDropAnimationPath(vector<osg::Vec3> path_Vec);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

//    osg::ref_ptr<osg::AnimationPath> generateAnimationPath(osg::ref_ptr<osg::Vec2Array>);


private slots:
    void dropClickSlot();
    void driftClickSlot();
    void routeClickSlot();
    void on_subject_TW_itemClicked(QTreeWidgetItem *item, int column);
    void on_parameter_TW_currentChanged(int index);
    void on_windDirection_DIAL_valueChanged(int value);
    void on_currentDirection_DIAL_valueChanged(int value);
    void on_parameter_TW_tabBarClicked(int index);
    void on_calculate_PB_clicked();
    void on_dropStart_BT_clicked();
    void on_VR_BT_clicked();
};

#endif // MAINWINDOW_H
