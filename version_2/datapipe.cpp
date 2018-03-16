#include "datapipe.h"

DataPipe::DataPipe(DataMap *map):inner_map(map)
{

}

void DataPipe::setDataMap(DataMap *map)
{
    inner_map = map;
}

MVec DataPair::getInner_windVec() const
{
    return inner_windVec;
}

void DataPair::setInner_windVec(const MVec &value)
{
    inner_windVec = value;
}

MVec DataPair::getInner_currentVec() const
{
    return inner_currentVec;
}

void DataPair::setInner_currentVec(const MVec &value)
{
    inner_currentVec = value;
}

DataPair DataPipe::getData(int lng, int lat)
{
    DataNode node = inner_map->getData(lng,lat);

    DataPair pair(node.getInner_windSpeed(),node.getInner_curentSpeed());

    return pair;
}
