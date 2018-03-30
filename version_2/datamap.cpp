#include "datamap.h"

DataMap::DataMap()
{
    inner_standardWind = new MVec2(rand()%10-5,rand()%10-5);
    inner_standardCurrent = new MVec2(rand()%10-5,rand()%10-5);

    connect(&inner_timer,SIGNAL(timeout()),this,SLOT(updateData()));
    inner_timer.start(10000);
    inner_maskHeight = 100;
    inner_maskWidth = 100;
}

/*
 * brief: the function which can return the wind and current data in the position(x,y)
 *
 * output: it will return the a DataNode, which include the wind and current data in position(x,y)
 */
DataNode DataMap::getData(int x, int y)
{
    int top = y - inner_maskHeight;
    int bottom = y + inner_maskHeight;
    int left = x - inner_maskWidth;
    int right = x + inner_maskWidth;

    int leftTopDis = INT_MAX,leftBottomDis = INT_MAX,rightTopDis = INT_MAX,rightBottomDis = INT_MAX;

    CurrentDataNode *_leftTopCurrentNode,*_leftBottomCurrentNode,*_rightTopCurrentNode,*_rightBottomCurrentNode;

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
        }
        if(pos_x<x&&pos_y>y && MVec2(x - pos_x,y - pos_y).length()<leftBottomDis){
            _leftBottomCurrentNode = &cur_currentNode;
        }
        if(pos_x>x&&pos_y<y && MVec2(x - pos_x,y - pos_y).length()<rightTopDis){
            _rightTopCurrentNode = &cur_currentNode;
        }
        if(pos_x>x&&pos_y>y && MVec2(x - pos_x,y - pos_y).length()<rightBottomDis){
            _rightBottomCurrentNode = &cur_currentNode;
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
        currentData = _leftTopCurrentNode->getInnerData() * leftTopPart/allPart +\
                _leftBottomCurrentNode->getInnerData() * leftBottomPart/allPart + \
                _rightTopCurrentNode->getInnerData() * rightTopPart/allPart + \
                _rightBottomCurrentNode->getInnerData() * rightBottomPart/allPart;
    }


    WindDataNode *_leftTopWindNode,*_leftBottomWindNode,*_rightTopWindNode,*_rightBottomWindNode;

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
        }
        if(pos_x<x&&pos_y>y && MVec2(x - pos_x,y - pos_y).length()<leftBottomDis){
            _leftBottomWindNode = &cur_windNode;
        }
        if(pos_x>x&&pos_y<y && MVec2(x - pos_x,y - pos_y).length()<rightTopDis){
            _rightTopWindNode = &cur_windNode;
        }
        if(pos_x>x&&pos_y>y && MVec2(x - pos_x,y - pos_y).length()<rightBottomDis){
            _rightBottomWindNode = &cur_windNode;
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
        windData =_leftTopWindNode->getInnerData() * leftTopPart/allPart +\
                _leftBottomWindNode->getInnerData() * leftBottomPart/allPart + \
                _rightTopWindNode->getInnerData() * rightTopPart/allPart + \
                _rightBottomWindNode->getInnerData() * rightBottomPart/allPart;

    }

    //calculate the orderless of wind in the four ivory  corner

    DataNode node(x,y);
    node.setInner_currentData(currentData);
    node.setInner_windData(windData);
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
void DataMap::updateData()
{
    updateCurrentData();
    updateWindData();
}

/*
 * brief: the update function of the current data
 *
 * input: it need to read a txt file, in this file there are all the current data but need to sort
 */
void DataMap::updateCurrentData()
{
    QFile file("current_20140717.txt");

    if(!file.open(QFile::ReadOnly)){
        qDebug()<<"open file error!";
    }

    while(!file.atEnd()){
        QByteArray line =  file.readLine();
        QString str(line);
        if(str.length() > 20){
            QStringList strList = str.split(' ');
            qDebug()<<strList.at(1)<<" "<<strList.at(2);
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
void DataMap::updateWindData()
{
    QFile file("wind_20140308.txt");

    if(!file.open(QFile::ReadOnly)){
        qDebug()<<"open file error!";
    }

    while(!file.atEnd()){
        QByteArray line =  file.readLine();
        QString str(line);
        if(str.length() > 20){
            QStringList strList = str.split(' ');
            qDebug()<<strList.at(0)<<" "<<strList.at(1);
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

/*
 * brief: the function used to calculate the postion correspond to the latitude and longtitude
 *
 * input: it need to input two double value of latitude and longtitude
 *
 * output: it will return a struct(Position) of x and y in the descrate coordinate
 */
Position DataMap::calculateellipse2plane(double B, double L)
{
    double l = 0, Lo = 0, a0 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, n = 0, c = 0;
    double x = 0, y = 0;
    double m = 0, p = 0, q = 0;
    int N = 0, i = 0; //带号

    N = int(dms2D(L) / 6);
    Lo = 6 * N - 3;

    l = (dms2D(L) - dms2D(Lo)) * 3600 / rho;  //单位为"
    B = dms2Rad(B);
    L = dms2Rad(L);
    Lo = dms2Rad(Lo);

    l = pow(l, 2);
    c = pow(cos(B), 2);  //c=cos(B)的平方
    n = 6399698.902 - (21562.267 - (108.973 - 0.612*c)*c)*c;
    a0 = 32140.404 - (135.3302 - (0.7092 - 0.0040*c)*c)*c;
    a3 = (0.3333333 + 0.001123*c)*c - 0.1666667;
    a4 = (0.25 + 0.00252*c)*c - 0.04166;
    a5 = 0.0083 - (0.1667 - (0.1968 + 0.004*c)*c)*c;
    a6 = (0.166*c - 0.084)*c;
    //计算平面坐标，并化为国家统一坐标
    m = sin(B)*cos(B);
    p = 1 + (a3 + a5*l)*l;
    x = 6367558.4969*B - (a0 - (0.5 + (a4 + a6*l)*l)*l*n)*m;
    y = (p)*sqrt(l)*n*cos(B);
    y = y + 500000 + N * 1000000;
    return Position(x/1000,y/1000);
}

double DataMap::dms2Rad(double Dms)
{
    double Degree, Miniute;
    double Second;
    int Sign;
    double Rad;
    if (Dms >= 0)
        Sign = 1;
    else
        Sign = -1;
    Dms = fabs(Dms);
    Degree = floor(Dms);
    Miniute = floor(fmod(Dms * 100.0, 100.0));
    Second = fmod(Dms * 10000.0, 100.0);
    Rad = Sign * (Degree + Miniute / 60.0 + Second / 3600.0) * M_PI / 180.0;
    return Rad;
}

double DataMap::dms2D(double Dms)
{
    double Degree, Miniute;
    double Second;
    int Sign;
    double D;
    if (Dms >= 0)
        Sign = 1;
    else
        Sign = -1;
    Dms = fabs(Dms);
    Degree = floor(Dms);
    Miniute = floor(fmod(Dms * 100.0, 100.0));
    Second = fmod(Dms * 10000.0, 100.0);
    D = Sign * (Degree + Miniute / 60.0 + Second / 3600.0);
    return D;
}

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
    }

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
    }

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
