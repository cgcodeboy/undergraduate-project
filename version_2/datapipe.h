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

    DataPair(MVec windVec,MVec currentVec):inner_windVec(windVec),inner_currentVec(currentVec){}

    MVec getInner_windVec() const;
    void setInner_windVec(const MVec &value);

    MVec getInner_currentVec() const;
    void setInner_currentVec(const MVec &value);

private:
    MVec inner_windVec;
    MVec inner_currentVec;
};

class DataPipe
{
public:
    DataPipe(DataMap *map = 0);

    void setDataMap(DataMap *map);

    DataPair getData(int lng,int lat);

private:
    DataMap *inner_map;
};

#endif // DATASOURCE_H
