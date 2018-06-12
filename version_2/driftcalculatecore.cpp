#include "driftcalculatecore.h"

/*
 * brief: the construction function of Calculate_Core
 */
DriftCalculateCore::DriftCalculateCore(COMPUTE_TYPE type):inner_type(type)
{

}

/*
 * brief: the getter of wind speed
 *
 * output: it will return the value of the speed of the wind
 */
MVec2 DriftCalculateCore::getInner_windSpeed() const
{
    return inner_windSpeed;
}

/*
 * brief: the setter of the wind speed
 *
 * input: it need a input of float value, which is the speed of the wind
 */
void DriftCalculateCore::setInner_windSpeed(MVec2 value)
{
    inner_windSpeed = value;
}

/*
 * brief: the getter of the current speed
 *
 * output: it will return the value of speed of the current
 */
MVec2 DriftCalculateCore::getInner_currentSpeed() const
{
    return inner_currentSpeed;
}

/*
 * brief: the setter of the current speed
 *
 * input: it need a input of float value, which is the speed of the current
 */
void DriftCalculateCore::setInner_currentSpeed(MVec2 value)
{
    inner_currentSpeed = value;
}

/*
 * brief: the getter of the leeway rate
 *
 * output: it will return the value of the leeway rate
 */
float DriftCalculateCore::getInner_leewayRate() const
{
    return inner_leewayRate;
}

/*
 * brief: the setter of the leeway rate
 *
 * input: it need a input of float value, which is the leeway rate
 */
void DriftCalculateCore::setInner_leewayRate(float value)
{
    inner_leewayRate = value;
}

/*
 * brief: the getter if the update frequency
 *
 * output: it will return the value of the update frequency
 */
int DriftCalculateCore::getInner_updateFrequency() const
{
    return inner_updateFrequency;
}

/*
 * brief: the setter of the update requency
 *
 * input: it need a input of float value, which is the frequency of updating the parameters
 */
void DriftCalculateCore::setInner_updateFrequency(int value)
{
    inner_updateFrequency = value;
}

/*
 * brief: the getter of the initial position of the object
 *
 * output: it will return the value of the initial position
 */
MVec2 DriftCalculateCore::getInner_initPosition() const
{
    if(inner_type == SIMPLE)
        return inner_initPosition;
    else
        return inner_coreDriftor.getInner_position();
}

/*
 * brief: the setter of the initial position of the object
 *
 * input: it need a input of MVec2, which is the initial position of the object
 */
void DriftCalculateCore::setInner_initPosition(const MVec2 &value)
{
    if(inner_type == SIMPLE)
        inner_initPosition = value;
    else{
        inner_coreDriftor.setInner_position(value);
    }
}

/*
 * brief: the setter of the spacial correlation
 *
 * input: it need a input of float value, which is the correlation of space
 */
void DriftCalculateCore::setInner_spacialCorrelation(float value)
{
    inner_spacialCorrelation = value;
}

void DriftCalculateCore::setInner_driftorVecSize(int value)
{
    inner_driftorVecSize = value;
    generateDriftorVec();
}

/*
 * brief: a function used to compute the B correlation between two point
 *
 * input: it need two MVec2 of point and two MVec2 of speed
 *
 * output: it will return the B correlation value between 0 to 1
 */
float DriftCalculateCore::compute_B_correlation(int index,VELOCITY_TYPE type)
{
    Driftor driftor = inner_driftorVec.at(index);
    MVec2 subPoint = driftor.getInner_position() - inner_coreDriftor.getInner_position();

    float square = subPoint.lengthSquare()/(inner_spacialCorrelation *inner_spacialCorrelation * 100 * 100);
    if(type == WIND){
        return exp(-(square/( compute_R_correlation(driftor.getInner_windSpeed(),inner_coreDriftor.getInner_windSpeed()))));
    }
    else{
        return exp(-(square/( compute_R_correlation(driftor.getInner_currentSpeed(),inner_coreDriftor.getInner_currentSpeed()))));
    }
}

/*
 * brief: a function used to compute the Eulerian  correlation between the speeds of two points
 *
 * input: it need two MVec2 of speed
 *
 * output: it will return the R correlation value
 */
float DriftCalculateCore::compute_R_correlation(MVec2 speed_1,MVec2 speed_2)
{
    float dotMul = MVec2::dot(speed_1,speed_2);

    float value = dotMul/(speed_1.length()*speed_2.length());

    return value;
}

MVec2 DriftCalculateCore::synthesisWindSpeed(MVec2 wind_speed)
{
    inner_coreDriftor.setInner_windSpeed(wind_speed);
    default_random_engine engine(time(0));
    uniform_real_distribution<float> uniform(0,1);

    vector<float> xVec;
    vector<float> yVec;
    vector<float> rVec;
    vector<float> thetaVec;

    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        xVec.push_back(uniform(engine));
        yVec.push_back(uniform(engine));
    }

    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        rVec.push_back(sqrtf(-2 * logf(xVec[i])));
        thetaVec.push_back(2 * M_PI * yVec[i]);
    }

    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        xVec[i] = rVec[i] * cosf(thetaVec[i]);
        yVec[i] = rVec[i] * sinf(thetaVec[i]);
    }

    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        Driftor& cur_driftor = inner_driftorVec.at(i);
        cur_driftor.setInner_windSpeed(MVec2(xVec[i] * inner_windDeviation * wind_speed.getX()+ wind_speed.getX(),yVec[i] * inner_windDeviation * wind_speed.getY() + wind_speed.getY()));
//        qDebug()<<cur_driftor.getInner_windSpeed().getX()<<" wind "<<cur_driftor.getInner_windSpeed().getY();
    }

    MVec2 sum;
    int count = 0;
    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        float b_value = compute_B_correlation(i,WIND);
        if(b_value>inner_spacialCorrelation){
            count++;
            Driftor cur_driftor = inner_driftorVec.at(i);
            sum = sum + cur_driftor.getInner_windSpeed();
        }
    }
//    qDebug()<<count<<" "<<wind_speed.getX()<<" "<<wind_speed.getY()<<" "<<sum.getX()<<" "<<sum.getY();
    return sum/count;
}

MVec2 DriftCalculateCore::synthesisCurrentSpeed(MVec2 current_speed)
{
    inner_coreDriftor.setInner_currentSpeed(current_speed);
    default_random_engine engine(time(0));
    uniform_real_distribution<float> uniform(0,1);

    vector<float> xVec;
    vector<float> yVec;
    vector<float> rVec;
    vector<float> thetaVec;

    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        xVec.push_back(uniform(engine));
        yVec.push_back(uniform(engine));
    }

    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        rVec.push_back(sqrtf(-2 * logf(xVec[i])));
        thetaVec.push_back(2 * M_PI * yVec[i]);
    }

    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        xVec[i] = rVec[i] * cosf(thetaVec[i]);
        yVec[i] = rVec[i] * sinf(thetaVec[i]);
    }

    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        Driftor& cur_driftor = inner_driftorVec.at(i);
        cur_driftor.setInner_currentSpeed(MVec2(xVec[i] * inner_currentDeviation * current_speed.getX()+ current_speed.getX(),yVec[i] * inner_currentDeviation * current_speed.getY()+ current_speed.getY()));
//        qDebug()<<cur_driftor.getInner_currentSpeed().getX()<<" current "<<cur_driftor.getInner_currentSpeed().getY();
    }

    MVec2 sum;
    int count = 0;
    for(unsigned int i = 0;i<inner_driftorVec.size();i++){
        float b_value = compute_B_correlation(i,CURRENT);
        if(b_value>inner_spacialCorrelation){
            count++;
            Driftor cur_driftor = inner_driftorVec.at(i);
            sum = sum + cur_driftor.getInner_currentSpeed();
        }
    }
//    qDebug()<<count<<" "<<current_speed.getX()<<" "<<current_speed.getY()<<" "<<sum.getX()<<" "<<sum.getY();
    return sum/static_cast<float>(count);
}

/*
 * brief: a function used to synthetize the object velocity using the wind velocity and current velocity
 *
 * input: it need input the wind velocity and current velocity
 */
MVec2 DriftCalculateCore::synthesisObjectSpeed()
{
    if(inner_type == SIMPLE){
        float object_SpeedX = inner_windSpeed.getX()*(1-this->inner_leewayRate) + inner_currentSpeed.getX()*this->inner_leewayRate;
        float object_SpeedY = inner_windSpeed.getY()*(1-this->inner_leewayRate) + inner_currentSpeed.getY()*this->inner_leewayRate;
        return MVec2(object_SpeedX,object_SpeedY);
    }else{
        DataPair curPair = inner_dataPipe.getData(inner_beginDate,inner_coreDriftor.getInner_position().getX(),inner_coreDriftor.getInner_position().getY());
        //qDebug()<<"pair"<<curPair.getInner_windVec().getX()<<" "<<curPair.getInner_windVec().getY()<<" "<<curPair.getInner_currentVec().getX()<<" "<<curPair.getInner_currentVec().getY();
        MVec2 sysWindSpeed = synthesisWindSpeed(curPair.getInner_windVec());
        MVec2 sysCurrentSpeed = synthesisCurrentSpeed(curPair.getInner_currentVec());
        float object_SpeedX = sysWindSpeed.getX()*(1-this->inner_leewayRate) + sysCurrentSpeed.getX()*this->inner_leewayRate;
        float object_SpeedY = sysWindSpeed.getY()*(1-this->inner_leewayRate) + sysCurrentSpeed.getY()*this->inner_leewayRate;
        return MVec2(object_SpeedX,object_SpeedY);
    }
}

//void DriftCalculateCore::updatePosition()
//{
//    for(unsigned int i = 0;i<inner_driftorVec.size();i++){
//        Driftor cur_driftor = inner_driftorVec.at(i);
//        float object_SpeedX = cur_driftor.getInner_windSpeed().getX()*(1-this->inner_leewayRate) + cur_driftor.getInner_currentSpeed().getX()*this->inner_leewayRate;
//        float object_SpeedY = cur_driftor.getInner_windSpeed().getY()*(1-this->inner_leewayRate) + cur_driftor.getInner_currentSpeed().getY()*this->inner_leewayRate;
//        MVec2 cur_position = cur_driftor.getInner_position();
//        cur_driftor.setInner_position(MVec2(cur_position.getX()+object_SpeedX * inner_updateFrequency * 86400,cur_position.getY()+object_SpeedY * inner_updateFrequency * 86400));
//    }
//}

void DriftCalculateCore::generateDriftorVec()
{
    inner_driftorVec.clear();
    MVec2 value = inner_coreDriftor.getInner_position();
    default_random_engine engine(time(0));
    int distance = inner_spacialCorrelation * 100;
    uniform_int_distribution<int> dis_vertival(value.getY()-distance,value.getY()+distance);
    uniform_int_distribution<int> dis_horizontal(value.getX()-distance,value.getX()+distance);
    for(int i = 0;i<inner_driftorVecSize;i++){
        Driftor *cur_driftor = new Driftor;
        cur_driftor->setInner_position(MVec2(dis_horizontal(engine),dis_vertival(engine)));
        inner_driftorVec.push_back(*cur_driftor);
    }
}

/*
 * brief: a function used to get the drift route, it contains the point in the route
 *
 * output: a vector contains all the point in the route
 */
vector<MVec2> DriftCalculateCore::getDriftRoute()
{
    if(inner_type == SIMPLE){
        return getSimpleDriftRoute();
    }
    else{
        return getComplexDriftRoute();
    }
}

vector<MVec2> DriftCalculateCore::getSimpleDriftRoute()
{
    vector<MVec2> routeVec;
    MVec2 object_Speed = synthesisObjectSpeed();
    int step = inner_simulateTime/inner_updateFrequency;
    for(int i = 0;i<step;i++){
        MVec2 vec(inner_initPosition.getX()+object_Speed.getX()*i,inner_initPosition.getY()+object_Speed.getY()*i);
        routeVec.push_back(vec);
    }

    return routeVec;
}

vector<MVec2> DriftCalculateCore::getComplexDriftRoute()
{
    vector<MVec2> routeVec;
    int step = inner_simulateTime/inner_updateFrequency;
    for(int i = 0;i<step;i++){
        MVec2 object_Speed = synthesisObjectSpeed();
        inner_beginDate = inner_beginDate.addDays(inner_updateFrequency);
        MVec2 cur_position = inner_coreDriftor.getInner_position();
        routeVec.push_back(cur_position);
        inner_coreDriftor.setInner_position(MVec2(cur_position.getX()+object_Speed.getX()*inner_updateFrequency*86400,cur_position.getY()+object_Speed.getY()*inner_updateFrequency*86400));
        generateDriftorVec();
    }
    return routeVec;
}

void DriftCalculateCore::setInner_coreDriftor(const Driftor &value)
{
    inner_coreDriftor = value;
}

void DriftCalculateCore::setDataPipe(const DataPipe &pipe)
{
    inner_dataPipe = pipe;
}

void DriftCalculateCore::setInner_simulateTime(int value)
{
    inner_simulateTime = value;
}

void DriftCalculateCore::setInner_windDeviation(float value)
{
    inner_windDeviation = value;
}

void DriftCalculateCore::setInner_currentDeviation(float value)
{
    inner_currentDeviation = value;
}

void DriftCalculateCore::setInner_beginDate(const QDate &value)
{
    inner_beginDate = value;
}

