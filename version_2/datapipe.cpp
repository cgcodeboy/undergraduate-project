#include "datapipe.h"

DataPipe::DataPipe(DataMap *map):inner_map(map)
{

}

void DataPipe::setDataMap(DataMap *map)
{
    inner_map = map;
}

MVec2 DataPair::getInner_windVec() const
{
    return inner_windVec;
}

void DataPair::setInner_windVec(const MVec2 &value)
{
    inner_windVec = value;
}

MVec2 DataPair::getInner_currentVec() const
{
    return inner_currentVec;
}

void DataPair::setInner_currentVec(const MVec2 &value)
{
    inner_currentVec = value;
}

DataPair DataPipe::getData(const QDate &date , int x, int y)
{
    DataNode node = inner_map->getData(date,x,y);

    DataPair pair(node.getInner_windData(),node.getInner_currentData());

    return pair;
}
