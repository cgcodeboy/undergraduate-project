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
MVec DriftCalculateCore::getInner_windSpeed() const
{
    return inner_windSpeed;
}

/*
 * brief: the setter of the wind speed
 *
 * input: it need a input of float value, which is the speed of the wind
 */
void DriftCalculateCore::setInner_windSpeed(MVec value)
{
    inner_windSpeed = value;
}

/*
 * brief: the getter of the current speed
 *
 * output: it will return the value of speed of the current
 */
MVec DriftCalculateCore::getInner_currentSpeed() const
{
    return inner_currentSpeed;
}

/*
 * brief: the setter of the current speed
 *
 * input: it need a input of float value, which is the speed of the current
 */
void DriftCalculateCore::setInner_currentSpeed(MVec value)
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
MVec DriftCalculateCore::getInner_initPosition() const
{
    return inner_initPosition;
}

/*
 * brief: the setter of the initial position of the object
 *
 * input: it need a input of MVec, which is the initial position of the object
 */
void DriftCalculateCore::setInner_initPosition(const MVec &value)
{
    inner_initPosition = value;
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

/*
 * brief: a function used to compute the B correlation between two point
 *
 * input: it need two MVec of point and two MVec of speed
 *
 * output: it will return the B correlation value between 0 to 1
 */
float DriftCalculateCore::compute_B_correlation(int index,VELOCITY_TYPE type)
{
    Driftor driftor = inner_driftorVec[index];
    MVec subPoint = driftor.getInner_position() - inner_coreDriftor.getInner_position();

    float square = subPoint.lengthSquare();
    if(type == WIND){
        return exp(-(square/(2* compute_R_correlation(driftor.getInner_windSpeed(),inner_coreDriftor.getInner_windSpeed()))));
    }
    else{
        return exp(-(square/(2* compute_R_correlation(driftor.getInner_currentSpeed(),inner_coreDriftor.getInner_currentSpeed()))));
    }
}

/*
 * brief: a function used to compute the Eulerian  correlation between the speeds of two points
 *
 * input: it need two MVec of speed
 *
 * output: it will return the R correlation value
 */
float DriftCalculateCore::compute_R_correlation(MVec speed_1,MVec speed_2)
{
    float dotMul = MVec::dot(speed_1,speed_2);

    return dotMul/(speed_1.length()*speed_2.length());
    return 0;
}

MVec DriftCalculateCore::synthesisWindSpeed(MVec wind_speed)
{
    default_random_engine engine;
    normal_distribution<float> normal(wind_speed.length(),inner_windDeviation);
    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        Driftor cur_driftor = inner_driftorVec.at(i);
        //wait for correcting...
        cur_driftor.setInner_windSpeed(MVec(normal(engine)*cosf(static_cast<float>(rand()%10)),normal(engine)*sinf(static_cast<float>(rand()%10))));
    }

    MVec sum;
    int count = 0;
    for(unsigned int i = 0;i < inner_driftorVec.size();i++){
        float b_value = compute_B_correlation(i,WIND);
        if(b_value>0.3){
            count++;
            Driftor cur_driftor = inner_driftorVec.at(i);
            sum = sum + cur_driftor.getInner_windSpeed();
        }
    }
    return sum/static_cast<float>(count);
}

MVec DriftCalculateCore::synthesisCurrentSpeed(MVec current_speed)
{
    default_random_engine engine;
    normal_distribution<float> normal(current_speed.length(),inner_currentDeviation);
    for(unsigned int i = 0;i<inner_driftorVec.size();i++){
        Driftor cur_driftor = inner_driftorVec.at(i);
        //wait for correcting...
        cur_driftor.setInner_currentSpeed(MVec(normal(engine)*cosf(static_cast<float>(rand()%10)),normal(engine)*sinf(static_cast<float>(rand()%10))));
    }

    MVec sum;
    int count = 0;
    for(unsigned int i = 0;i<inner_driftorVec.size();i++){
        float b_value = compute_B_correlation(i,CURRENT);
        if(b_value>0.3){
            count++;
            Driftor cur_driftor = inner_driftorVec.at(i);
            sum = sum + cur_driftor.getInner_currentSpeed();
        }
    }
    return sum/static_cast<float>(count);
}

/*
 * brief: a function used to synthetize the object velocity using the wind velocity and current velocity
 *
 * input: it need input the wind velocity and current velocity
 */
MVec DriftCalculateCore::synthesisObjectSpeed()
{
    if(inner_type == SIMPLE){
        float object_SpeedX = inner_windSpeed.getX()*(1-this->inner_leewayRate) + inner_currentSpeed.getX()*this->inner_leewayRate;
        float object_SpeedY = inner_windSpeed.getY()*(1-this->inner_leewayRate) + inner_currentSpeed.getY()*this->inner_leewayRate;
        return MVec(object_SpeedX,object_SpeedY);
    }else{
        DataPair curPair = inner_dataPipe.getData(inner_coreDriftor.getInner_position().getX(),inner_coreDriftor.getInner_position().getY());
        float object_SpeedX = synthesisWindSpeed(curPair.getInner_windVec()).getX()*(1-this->inner_leewayRate) + synthesisCurrentSpeed(curPair.getInner_currentVec()).getX()*this->inner_leewayRate;
        float object_SpeedY = synthesisWindSpeed(curPair.getInner_windVec()).getY()*(1-this->inner_leewayRate) + synthesisCurrentSpeed(curPair.getInner_currentVec()).getY()*this->inner_leewayRate;
        return MVec(object_SpeedX,object_SpeedY);
    }
}

void DriftCalculateCore::updatePosition()
{
    for(unsigned int i = 0;i<inner_driftorVec.size();i++){
        Driftor cur_driftor = inner_driftorVec.at(i);
        float object_SpeedX = cur_driftor.getInner_windSpeed().getX()*(1-this->inner_leewayRate) + cur_driftor.getInner_currentSpeed().getX()*this->inner_leewayRate;
        float object_SpeedY = cur_driftor.getInner_windSpeed().getY()*(1-this->inner_leewayRate) + cur_driftor.getInner_currentSpeed().getY()*this->inner_leewayRate;
        MVec cur_position = cur_driftor.getInner_position();
        cur_driftor.setInner_position(MVec(cur_position.getX()+object_SpeedX,cur_position.getY()+object_SpeedY));
    }
}

/*
 * brief: a function used to get the drift route, it contains the point in the route
 *
 * output: a vector contains all the point in the route
 */
vector<MVec> DriftCalculateCore::getDriftRoute()
{
    if(inner_type == SIMPLE){
        return getSimpleDriftRoute();
    }
    else{
        return getComplexDriftRoute();
    }
}

vector<MVec> DriftCalculateCore::getSimpleDriftRoute()
{
    vector<MVec> routeVec;
    MVec object_Speed = synthesisObjectSpeed();
    int step = inner_simulateTime/inner_updateFrequency;
    for(int i = 0;i<step;i++){
        MVec vec(inner_initPosition.getX()+object_Speed.getX()*i,inner_initPosition.getY()+object_Speed.getY()*i);
        routeVec.push_back(vec);
    }
    return routeVec;
}

vector<MVec> DriftCalculateCore::getComplexDriftRoute()
{
    vector<MVec> routeVec;
    int step = inner_simulateTime/inner_updateFrequency;
    for(int i = 0;i<step;i++){
        MVec object_Speed = synthesisObjectSpeed();
        MVec vec(inner_initPosition.getX()+object_Speed.getX()*i,inner_initPosition.getY()+object_Speed.getY()*i);
        routeVec.push_back(vec);
        MVec cur_position = inner_coreDriftor.getInner_position();
        inner_coreDriftor.setInner_position(MVec(cur_position.getX()+object_Speed.getX(),cur_position.getY()+object_Speed.getY()));
        updatePosition();
    }
    return routeVec;
}

void DriftCalculateCore::setInnder_driftorVec(const vector<Driftor> &value)
{
    //    innder_driftorVec = value;
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

