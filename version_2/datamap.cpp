#include "datamap.h"

DataMap::DataMap()
{
    inner_standardWind = new MVec2(rand()%10-5,rand()%10-5);
    inner_standardCurrent = new MVec2(rand()%10-5,rand()%10-5);

    inner_maskHeight = 200000;
    inner_maskWidth = 200000;
}

/*
 * brief: the function which can return the wind and current data in the position(x,y)
 *
 * output: it will return the a DataNode, which include the wind and current data in position(x,y)
 */
DataNode DataMap::getData(const QDate &date, int x, int y)
{
    updateTrigger(date);
    int top = y - inner_maskHeight;
    int bottom = y + inner_maskHeight;
    int left = x - inner_maskWidth;
    int right = x + inner_maskWidth;

    int leftTopDis = INT_MAX,leftBottomDis = INT_MAX,rightTopDis = INT_MAX,rightBottomDis = INT_MAX;

    CurrentDataNode *_leftTopCurrentNode = nullptr,*_leftBottomCurrentNode = nullptr,*_rightTopCurrentNode = nullptr,*_rightBottomCurrentNode = nullptr;

    float leftTopOrderless;
    float leftBottomOrderless;
    float rightTopOrderless;
    float rightBottomOrderless;

    float leftTopPart;
    float leftBottomPart;
    float rightTopPart;
    float rightBottomPart;
    float allPart;

    vector<CurrentDataNode> *cur_currentVec = new vector<CurrentDataNode>;

    //travel all the current node, get call the node in the mask and the four node which is the nearest to the
    //postion(x,y) in the four corner
    for(int i = 0;i<inner_currentNodeVec.size();i++){
        CurrentDataNode cur_currentNode = inner_currentNodeVec[i];
        float pos_x = cur_currentNode.getInner_x();
        float pos_y = cur_currentNode.getInner_y();
        if(pos_x > left && pos_x < right && pos_y > top && pos_y < bottom){          
            cur_currentVec->push_back(cur_currentNode);
        }
        if(pos_x<x&&pos_y<y && MVec2(x - pos_x,y - pos_y).length()<leftTopDis){
            _leftTopCurrentNode = &cur_currentNode;
            leftTopDis = MVec2(x - pos_x,y - pos_y).length();
        }
        if(pos_x<x&&pos_y>y && MVec2(x - pos_x,y - pos_y).length()<leftBottomDis){
            _leftBottomCurrentNode = &cur_currentNode;
            leftBottomDis = MVec2(x - pos_x,y - pos_y).length();
        }
        if(pos_x>x&&pos_y<y && MVec2(x - pos_x,y - pos_y).length()<rightTopDis){
            _rightTopCurrentNode = &cur_currentNode;
            rightTopDis = MVec2(x - pos_x,y - pos_y).length();
        }
        if(pos_x>x&&pos_y>y && MVec2(x - pos_x,y - pos_y).length()<rightBottomDis){
            _rightBottomCurrentNode = &cur_currentNode;
            rightBottomDis =  MVec2(x - pos_x,y - pos_y).length();
        }
    }
    MVec2 currentData(0,0);
    if(cur_currentVec->size() == 0){
        for(int i = 0;i<inner_currentNodeVec.size();i++){
            CurrentDataNode cur_currentNode = inner_currentNodeVec[i];
            currentData = currentData + cur_currentNode.getInnerData();
        }
        currentData = currentData / inner_currentNodeVec.size();
    }
    else{
        //calculate the orderless of current in the four ivory  corner
        leftTopOrderless = calculateCurrentOrderless(top,y,left,x,cur_currentVec);
        leftBottomOrderless = calculateCurrentOrderless(y,bottom,left,x,cur_currentVec);
        rightTopOrderless = calculateCurrentOrderless(top,y,x,right,cur_currentVec);
        rightBottomOrderless = calculateCurrentOrderless(y,bottom,x,right,cur_currentVec);

        //sysnthesis the Distance factor with the orderless factor
        leftTopPart = leftTopDis * leftTopOrderless;
        leftBottomPart = leftBottomDis * leftBottomOrderless;
        rightTopPart = rightTopDis * rightTopOrderless;
        rightBottomPart = rightBottomDis * rightBottomOrderless;

        allPart = leftTopPart + leftBottomPart + rightTopPart + rightBottomPart;

        //systhesis the final current data in position(x,y)
        if(_leftTopCurrentNode){
            currentData = currentData + _leftTopCurrentNode->getInnerData() * leftTopPart/allPart;
        }
        if(_leftBottomCurrentNode){
            currentData = currentData + _leftBottomCurrentNode->getInnerData() * leftBottomPart/allPart;
        }
        if(_rightTopCurrentNode){
            currentData = currentData + _rightTopCurrentNode->getInnerData() * rightTopPart/allPart;
        }
        if(_rightBottomCurrentNode){
            currentData = currentData + _rightBottomCurrentNode->getInnerData() * rightBottomPart/allPart;
        }
    }
    WindDataNode *_leftTopWindNode,*_leftBottomWindNode,*_rightTopWindNode,*_rightBottomWindNode;

    leftTopDis = INT_MAX,leftBottomDis = INT_MAX,rightTopDis = INT_MAX,rightBottomDis = INT_MAX;
    vector<WindDataNode> *cur_windVec = new vector<WindDataNode>;

    //travel all the wind node, get call the node in the mask and the four node which is the nearest to the
    //postion(x,y) in the four corner
    for(int i = 0;i<inner_windNodeVec.size();i++){
        WindDataNode cur_windNode = inner_windNodeVec[i];
        float pos_x = cur_windNode.getInner_x();
        float pos_y = cur_windNode.getInner_y();
        if(pos_x > left && pos_x < right && pos_y > top && pos_y < bottom){
            cur_windVec->push_back(cur_windNode);
        }
        if(pos_x<x&&pos_y<y && MVec2(x - pos_x,y - pos_y).length()<leftTopDis){
            _leftTopWindNode = &cur_windNode;
            leftTopDis = MVec2(x - pos_x,y - pos_y).length();
        }
        if(pos_x<x&&pos_y>y && MVec2(x - pos_x,y - pos_y).length()<leftBottomDis){
            _leftBottomWindNode = &cur_windNode;
            leftBottomDis = MVec2(x - pos_x,y - pos_y).length();
        }
        if(pos_x>x&&pos_y<y && MVec2(x - pos_x,y - pos_y).length()<rightTopDis){
            _rightTopWindNode = &cur_windNode;
            rightTopDis = MVec2(x - pos_x,y - pos_y).length();
        }
        if(pos_x>x&&pos_y>y && MVec2(x - pos_x,y - pos_y).length()<rightBottomDis){
            _rightBottomWindNode = &cur_windNode;
            rightBottomDis =  MVec2(x - pos_x,y - pos_y).length();
        }
    }
    MVec2 windData(0,0);
    if(cur_windVec->size() == 0){
        for(int i = 0;i<inner_windNodeVec.size();i++){
            WindDataNode cur_windNode = inner_windNodeVec[i];
            windData = windData + cur_windNode.getInnerData();
        }
        windData = windData / inner_windNodeVec.size();
    }
    else{
        //calculate the orderless of wind in the four ivory  corner
        leftTopOrderless = calculateWindOrderless(top,y,left,x,cur_windVec);
        leftBottomOrderless = calculateWindOrderless(y,bottom,left,x,cur_windVec);
        rightTopOrderless = calculateWindOrderless(top,y,x,right,cur_windVec);
        rightBottomOrderless = calculateWindOrderless(y,bottom,x,right,cur_windVec);

        //sysnthesis the Distance factor with the orderless factor
        leftTopPart = leftTopDis * leftTopOrderless;
        leftBottomPart = leftBottomDis * leftBottomOrderless;
        rightTopPart = rightTopDis * rightTopOrderless;
        rightBottomPart = rightBottomDis * rightBottomOrderless;

        allPart = leftTopPart + leftBottomPart + rightTopPart + rightBottomPart;
        //systhesis the final current data in position(x,y)
        if(_leftTopWindNode){
            windData = windData + _leftTopWindNode->getInnerData() * leftTopPart/allPart;
        }
        if(_leftBottomWindNode){
            windData = windData + _leftBottomWindNode->getInnerData() * leftBottomPart/allPart;
        }
        if(_rightTopWindNode){
            windData = windData + _rightTopWindNode->getInnerData() * rightTopPart/allPart;
        }
        if(_rightBottomWindNode){
            windData = windData + _rightBottomWindNode->getInnerData() * rightBottomPart/allPart;
        }
    }

    DataNode node(x,y);
    node.setInner_currentData(currentData);
    node.setInner_windData(windData);

    delete cur_currentVec,cur_windVec,_leftTopCurrentNode,_leftBottomCurrentNode,_rightTopCurrentNode,_rightBottomCurrentNode,_leftTopWindNode,_leftBottomWindNode,_rightTopWindNode,_rightBottomWindNode;
    return node;
}

/*
 * brief: the setter of the mask
 *
 * input: it need a input two int value, which is the width and height of the mask
 */
void DataMap::setMaskSize(int width, int height)
{
    inner_maskWidth = width;
    inner_maskHeight = height;
}

/*
 * brief: a update function
 *
 * no input and output
 *
 * in this function, current data and wind data will be updated
 */
//void DataMap::updateData()
//{
//    updateCurrentData();
//    updateWindData();
//}

void DataMap::setCurrentSourceFileNameList(const QStringList &value)
{
    currentSourceFileNameList = value;
}

void DataMap::setWindSourceFileNameList(const QStringList &value)
{
    windSourceFileNameList = value;
}

QString DataMap::getSourcePath() const
{
    return sourcePath;
}

void DataMap::setSourcePath(const QString &value)
{
    sourcePath = value;
}

/*
 * brief: the update function of the current data
 *
 * input: it need to read a txt file, in this file there are all the current data but need to sort
 */
void DataMap::updateCurrentData(QString fileName)
{
    qDebug()<<fileName<<"read";
    QFile file(fileName);

    if(!file.open(QFile::ReadOnly)){
        qDebug()<<"open file error!";
    }

    while(!file.atEnd()){
        QByteArray line =  file.readLine();
        QString str(line);
        if(str.length() > 20){
            QStringList strList = str.split(' ');
            QString lat = strList.at(1);
            QString lon  = strList.at(2);
            Position pos = calculateellipse2plane(lat.toFloat(),lon.toFloat());
            CurrentDataNode node(pos.x,pos.y);
            node.setInnerData(MVec2(QString(strList.at(3)).toFloat(),QString(strList.at(4)).toFloat()));
            inner_currentNodeVec.push_back(node);
        }
    }
}

/*
 * brief: the update function of the wind data
 *
 * input: it need to read a txt file, in this file there are all the wind data but need to sort
 */
void DataMap::updateWindData(QString fileName)
{
    QFile file(fileName);

    if(!file.open(QFile::ReadOnly)){
        qDebug()<<"open file error!";
    }

    while(!file.atEnd()){
        QByteArray line =  file.readLine();
        QString str(line);
        if(str.length() > 20){
            QStringList strList = str.split(' ');
            QString lat = strList.at(0);
            QString lon  = strList.at(1);
            Position pos = calculateellipse2plane(lat.toFloat(),lon.toFloat());
            WindDataNode node(pos.x,pos.y);

            float angle = strList.at(2).toFloat();
            float windSpeed = strList.at(3).toFloat();
            float windDataHorizontal = cosf(angle) * windSpeed;
            float windDataVertical = sinf(angle) * windSpeed;

            node.setInnerData(MVec2(windDataHorizontal,windDataVertical));
            inner_windNodeVec.push_back(node);
        }
    }
}

void DataMap::updateTrigger(const QDate &date)
{
    QString latesStr;
    for(int i = 0;i<windSourceFileNameList.size();i++){
        QString windSourceName = windSourceFileNameList.at(i);
        QDate curDate(windSourceName.mid(5,4).toInt(),windSourceName.mid(9,2).toInt(),windSourceName.mid(11,2).toInt());
        if(curDate<=date){
            latesStr = windSourceName;
        }
    }
    if(latesStr != curUseWindFile){
        updateWindData(latesStr);
    }

    for(int i = 0;i<currentSourceFileNameList.size();i++){
        QString currentSourceName = currentSourceFileNameList.at(i);
        QDate curDate(currentSourceName.mid(8,4).toInt(),currentSourceName.mid(12,2).toInt(),currentSourceName.mid(14,2).toInt());
        if(curDate<=date){
            latesStr = currentSourceName;
        }
    }
    if(latesStr != curUseCurrentFile){
        updateCurrentData(latesStr);
    }
}

/*
 * brief: the function used to calculate the postion correspond to the latitude and longtitude
 *
 * input: it need to input two double value of latitude and longtitude
 *
 * output: it will return a struct(Position) of x and y in the descrate coordinate
 */


/*
 * brief: the function used for calculating the current orderless in the four corner
 *
 * input: it need to input four int value of a corner and the current node vector which is in the mask
 *
 * output: it will return the orderless value of current
 */
float DataMap::calculateCurrentOrderless(int top, int bottom, int left, int right, vector<CurrentDataNode> *dataVec)
{
    MVec2 sumVec(0,0);
    int count = 0;
    for(int i = 0;i<dataVec->size();i++){
        CurrentDataNode node = dataVec->at(i);
        int x = node.getInner_x();
        int y = node.getInner_y();
        if(x>left&&x<right&&y>top&&y<bottom){
            sumVec = sumVec + node.getInnerData();
            count ++;
        }
    }
    if(count !=0){
        sumVec = sumVec / count;
        float orderless = 0;
        float sumLength = sumVec.length();
        for(int i = 0;i<dataVec->size();i++){
            CurrentDataNode node = dataVec->at(i);
            int x = node.getInner_x();
            int y = node.getInner_y();
            if(x>left&&x<right&&y>top&&y<bottom){
                MVec2 vecCur = node.getInnerData();
                float crossDot = MVec2::dot(vecCur,sumVec);
                orderless += acosf(crossDot /(vecCur.length()* sumLength));
            }
        }

        return orderless /= count;
    }
    else{
        return 0;
    }
}

/*
 * brief: the function used for calculating the wind orderless in the four corner
 *
 * input: it need to input four int value of a corner and the wind node vector which is in the mask
 *
 * output: it will return the orderless value of wind
 */
float DataMap::calculateWindOrderless(int top, int bottom, int left, int right, vector<WindDataNode> *dataVec)
{
    MVec2 sumVec(0,0);
    int count = 0;
    for(int i = 0;i<dataVec->size();i++){
        WindDataNode node = dataVec->at(i);
        int x = node.getInner_x();
        int y = node.getInner_y();
        if(x>left&&x<right&&y>top&&y<bottom){
            sumVec = sumVec + node.getInnerData();
            count ++;
        }
    }
    if(count !=0){
        sumVec = sumVec / count;
        float orderless = 0;
        float sumLength = sumVec.length();
        for(int i = 0;i<dataVec->size();i++){
            WindDataNode node = dataVec->at(i);
            int x = node.getInner_x();
            int y = node.getInner_y();
            if(x>left&&x<right&&y>top&&y<bottom){
                MVec2 vecCur = node.getInnerData();
                float crossDot = MVec2::dot(vecCur,sumVec);
                orderless += acosf(crossDot /(vecCur.length()* sumLength));
            }
        }

        return orderless /= count;
    }
    else{
        return 0;
    }
}

int DataNode::getInner_y() const
{
    return inner_y;
}

int DataNode::getInner_x() const
{
    return inner_x;
}

MVec2 DataNode::getInner_windData() const
{
    return inner_windData;
}

void DataNode::setInner_windData(const MVec2 &value)
{
    inner_windData = value;
}

MVec2 DataNode::getInner_currentData() const
{
    return inner_currentData;
}

void DataNode::setInner_currentData(const MVec2 &value)
{
    inner_currentData = value;
}

CurrentDataNode::CurrentDataNode(int _x, int _y):DataNode(_x,_y)
{

}

MVec2 CurrentDataNode::getInnerData()
{
    return getInner_currentData();
}

void CurrentDataNode::setInnerData(const MVec2 &value)
{
    setInner_currentData(value);
}

WindDataNode::WindDataNode(int _x, int _y):DataNode(_x,_y)
{

}

void WindDataNode::setInnerData(const MVec2 &value)
{
    setInner_windData(value);
}

MVec2 WindDataNode::getInnerData()
{
    return getInner_windData();
}
