#ifndef DATASOURCE_H
#define DATASOURCE_H

//////////////////
/// headers
#include <vector>
#include <queue>

#include <all_include.h>
#include <datamap.h>
using namespace std;
//////////////////
class DataPair
{
public:
    DataPair() = default;

    DataPair(MVec2 windVec,MVec2 currentVec):inner_windVec(windVec),inner_currentVec(currentVec){}

    MVec2 getInner_windVec() const;
    void setInner_windVec(const MVec2 &value);

    MVec2 getInner_currentVec() const;
    void setInner_currentVec(const MVec2 &value);

private:
    MVec2 inner_windVec;
    MVec2 inner_currentVec;
};

class DataPipe
{
public:
    DataPipe(DataMap *map = 0);

    void setDataMap(DataMap *map);

    DataPair getData(const QDate &date, int x, int y);

private:
    DataMap *inner_map;
};

#endif // DATASOURCE_H
