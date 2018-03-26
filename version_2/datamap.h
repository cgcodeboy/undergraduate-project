#ifndef DATAMAP_H
#define DATAMAP_H

//////////////////
/// headers

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QtMath>

#include <vector>

#include <all_include.h>

#define rho 206265
using namespace std;
//////////////////
/// \brief The DataNode class
//////////////////

typedef struct Position{
    float x;
    float y;
    Position(float _x,float _y):x(_x),y(_y){}
}Position;

class DataNode{
public:
    DataNode(int x,int y):inner_x(x),inner_y(y){}

    MVec2 getInner_windSpeed() const;
    void setInner_windSpeed(const MVec2 &value);

    MVec2 getInner_curentSpeed() const;
    void setInner_curentSpeed(const MVec2 &value);

    int getInner_y() const;

    int getInner_x() const;

private:
    int inner_y;
    int inner_x;

    MVec2 inner_windSpeed;
    MVec2 inner_curentSpeed;
};

class DataMap:public QObject
{
    Q_OBJECT
public:
    DataMap();

    DataNode getData(int x, int y) const;

private slots:
    void updateData();

private:
    vector<DataNode> inner_nodeVec;
    MVec2 *inner_standardWind;
    MVec2 *inner_standardCurrent;
    QTimer inner_timer;

    void updateCurrentData();
    void updateWindData();

    Position calculateellipse2plane(double B, double L);
    double dms2Rad(double Dms);
    double dms2D(double Dms);
};

#endif // DATAMAP_H
