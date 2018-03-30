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


//////////////////
/// \brief The DataNode class
//////////////////
class DataNode{
public:
    DataNode(int _x,int _y):inner_x(_x),inner_y(_y){}

    int getInner_y() const;

    int getInner_x() const;

    MVec2 getInner_windData() const;
    void setInner_windData(const MVec2 &value);

    MVec2 getInner_currentData() const;
    void setInner_currentData(const MVec2 &value);

private:
    int inner_y;
    int inner_x;

    MVec2 inner_windData;
    MVec2 inner_currentData;
};


/////////////////
/// \brief The CurrentDataNode class, a class inherit from DataNode
/////////////////
class CurrentDataNode:public DataNode{
public:
    CurrentDataNode(int _x, int _y);

    MVec2 getInnerData();

    void setInnerData(const MVec2 &value);

private:
    MVec2 inner_currentData;
};


//////////////////
/// \brief The WindDataNode class, a class inherit from DataNode
//////////////////
class WindDataNode:public DataNode{
public:
    WindDataNode(int _x, int _y);

    void setInnerData(const MVec2 &value);

    MVec2 getInnerData();

private:
    MVec2 inner_windData;
};


class DataMap:public QObject
{
    Q_OBJECT
public:
    DataMap();

    DataNode getData(int x, int y);

    void setMaskSize(int width,int height);

private slots:
    void updateData();

private:
//    enum NodeType{
//        CURRENT_TYPE,
//        WIND_TYPE
//    };

    vector<WindDataNode> inner_windNodeVec;
    vector<CurrentDataNode> inner_currentNodeVec;
    MVec2 *inner_standardWind;
    MVec2 *inner_standardCurrent;
    QTimer inner_timer;

    int inner_maskWidth;
    int inner_maskHeight;

    void updateCurrentData();
    void updateWindData();

    Position calculateellipse2plane(double B, double L);
    double dms2Rad(double Dms);
    double dms2D(double Dms);

    float calculateCurrentOrderless(int top, int bottom, int left, int right, vector<CurrentDataNode> *dataVec);
    float calculateWindOrderless(int top, int bottom, int left, int right, vector<WindDataNode> *dataVec);
};

#endif // DATAMAP_H
