#include "datamap.h"

DataMap::DataMap()
{
    inner_standardWind = new MVec2(rand()%10-5,rand()%10-5);
    inner_standardCurrent = new MVec2(rand()%10-5,rand()%10-5);

    connect(&inner_timer,SIGNAL(timeout()),this,SLOT(updateData()));
    inner_timer.start(10000);
}

DataNode DataMap::getData(int x, int y) const
{
//    for(unsigned i = 0;i<inner_nodeVec.size();i++){
//        DataNode node = inner_nodeVec.at(i);
//        if(node.getInner_y() == y&&node.getInner_x() == x){
//            return node;
//        }
//    }

    //Bilinear Interpolation to get the useful data

}

void DataMap::updateData()
{

}

void DataMap::updateCurrentData()
{
    double B = 0, L = 0;
    double x = 0, y = 0;
    int i = 0;

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
            DataNode node(pos.x,pos.y);
            node.setInner_curentSpeed(MVec2(QString(strList.at(3)).toFloat(),QString(strList.at(4)).toFloat()));
            inner_nodeVec.push_back(node);
        }
    }
}

void DataMap::updateWindData()
{

}

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


MVec2 DataNode::getInner_windSpeed() const
{
    return inner_windSpeed;
}

void DataNode::setInner_windSpeed(const MVec2 &value)
{
    inner_windSpeed = value;
}

MVec2 DataNode::getInner_curentSpeed() const
{
    return inner_curentSpeed;
}

void DataNode::setInner_curentSpeed(const MVec2 &value)
{
    inner_curentSpeed = value;
}

int DataNode::getInner_y() const
{
    return inner_y;
}

int DataNode::getInner_x() const
{
    return inner_x;
}
