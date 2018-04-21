#ifndef DATAMAP_H
#define DATAMAP_H

//////////////////
/// headers

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QtMath>
#include <QDate>

#include <vector>

#include <all_include.h>

using namespace std;
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

    void setMaskSize(int width,int height);

    QString getSourcePath() const;
    void setSourcePath(const QString &value);

    DataNode getData(const QDate &date, int x, int y);

    void setWindSourceFileNameList(const QStringList &value);

    void setCurrentSourceFileNameList(const QStringList &value);

private:

    vector<WindDataNode> inner_windNodeVec;
    vector<CurrentDataNode> inner_currentNodeVec;
    MVec2 *inner_standardWind;
    MVec2 *inner_standardCurrent;
    QString sourcePath;
    QStringList windSourceFileNameList;
    QStringList currentSourceFileNameList;
    QString curUseWindFile;
    QString curUseCurrentFile;

    int inner_maskWidth;
    int inner_maskHeight;

    void updateCurrentData(QString fileName);
    void updateWindData(QString fileName);

    void updateTrigger(const QDate &date);

    float calculateCurrentOrderless(int top, int bottom, int left, int right, vector<CurrentDataNode> *dataVec);
    float calculateWindOrderless(int top, int bottom, int left, int right, vector<WindDataNode> *dataVec);
};

#endif // DATAMAP_H
