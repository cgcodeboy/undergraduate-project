#include "datamap.h"

DataMap::DataMap()
{
    inner_standardWind = new MVec(rand()%10-5,rand()%10-5);
    inner_standardCurrent = new MVec(rand()%10-5,rand()%10-5);

    connect(&inner_timer,SIGNAL(timeout()),this,SLOT(updateData()));
    inner_timer.start(10000);
}

DataNode DataMap::getData(int lng, int lat) const
{
    for(int i = 0;i<inner_nodeVec.size();i++){
        DataNode node = inner_nodeVec.at(i);
        if(node.getInner_lng() == lat&&node.getInner_lat() == lat){
            return node;
        }
    }
}

void DataMap::updateData()
{
//    MVec windVec()
//    for(int i = 0;i<inner_nodeVec.size();i++){
//        DataNode node = inner_nodeVec.at(i);
//        node.setInner_windSpeed();
//    }
}


MVec DataNode::getInner_windSpeed() const
{
    return inner_windSpeed;
}

void DataNode::setInner_windSpeed(const MVec &value)
{
    inner_windSpeed = value;
}

MVec DataNode::getInner_curentSpeed() const
{
    return inner_curentSpeed;
}

void DataNode::setInner_curentSpeed(const MVec &value)
{
    inner_curentSpeed = value;
}

int DataNode::getInner_lng() const
{
    return inner_lng;
}

int DataNode::getInner_lat() const
{
    return inner_lat;
}
