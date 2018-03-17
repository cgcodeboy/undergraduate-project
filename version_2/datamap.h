#ifndef DATAMAP_H
#define DATAMAP_H

//////////////////
/// headers

#include <QObject>
#include <QTimer>

#include <vector>

#include <all_include.h>
using namespace std;
//////////////////

class DataNode{
public:
    DataNode(int lng,int lat):inner_lat(lat),inner_lng(lng){}

    MVec2 getInner_windSpeed() const;
    void setInner_windSpeed(const MVec2 &value);

    MVec2 getInner_curentSpeed() const;
    void setInner_curentSpeed(const MVec2 &value);

    int getInner_lng() const;

    int getInner_lat() const;

private:
    int inner_lng;
    int inner_lat;

    MVec2 inner_windSpeed;
    MVec2 inner_curentSpeed;
};

class DataMap:public QObject
{
    Q_OBJECT
public:
    DataMap();

    DataNode getData(int lng,int lat) const;

private slots:
    void updateData();

private:
    vector<DataNode> inner_nodeVec;
    MVec2 *inner_standardWind;
    MVec2 *inner_standardCurrent;
    QTimer inner_timer;
};

#endif // DATAMAP_H
