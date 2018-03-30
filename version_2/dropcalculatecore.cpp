#include "dropcalculatecore.h"

DropcalculateCore::DropcalculateCore()
{

}

/*
 * brief: the setter of wind weight
 *
 * input: it need input a int value, which is the weight of the airplane
 */
void DropcalculateCore::setInner_weight(int value)
{
    inner_weight = value;
}

/*
 * brief: the setter of wind speed
 *
 * input: it need input a MVec3, which is the initial speed of the airplane
 */
void DropcalculateCore::setInner_flySpeed(const MVec3 &value)
{
    inner_flySpeed = value;
}

/*
 * brief: the setter of wing area
 *
 * input: it need to input a float value, which is the area of the airplane's wing
 */
void DropcalculateCore::setInner_wingArea(float value)
{
    inner_wingArea = value;
}

/*
 * brief: the setter of all area
 *
 * output: it need to input a float value, which is the all area of the airplane
 */
void DropcalculateCore::setInner_airplaneArea(float value)
{
    inner_airplaneArea = value;
}

/*
 * brief: the setter of wind speed
 *
 * output: it need to input a MVec3, which is the initial position of the airplane
 */
void DropcalculateCore::setInner_Position(const MVec3 &value)
{
    inner_Position = value;
}

/*
 * brief: the function which can return the drop route calculated with the parameter of position、speed and eta
 *
 * output: it will return the a vector of MVec3, which include the discrete position in the route
 */
vector<MVec3> DropcalculateCore::getDropRoute()
{
    vector<MVec3> routeVec;
    routeVec.push_back(inner_Position);
    MVec3 position(inner_Position);
    MVec3 speed(inner_flySpeed);
    while(position.getZ()>0){
        MVec3 compositonForce = generateCompositionForces(speed,position.getZ());
        MVec3 accelerator = generateAccelerator(compositonForce);       
        speed = calculteVelocity(speed,accelerator);
        position = calculatePosition(speed,position);
        routeVec.push_back(position);
    }
    return routeVec;
}

float DropcalculateCore::calculatePressure(float _height)
{
    float cur = 1 - (L * _height)/T_0;
    float exponent = (g * M)/(R * L);
    return p_0 * exp(exponent * log(cur));
}

float DropcalculateCore::calcalateAirDensity(float _height)
{
    float p = calculatePressure(_height);
    float T = calculateTemperature(_height);
    float cur = (p * M)/(R * T);
    return cur;
}

float DropcalculateCore::calculateTemperature(float _height)
{
    return T_0 - L * _height;
}

MVec3 DropcalculateCore::generateGravity()
{
    return MVec3(0,0, -inner_weight * g);
}

MVec3 DropcalculateCore::generateLift(const MVec3& _speed,float _height)
{
    return MVec3(0,0,0.5 * calcalateAirDensity(_height) * inner_wingArea * _speed.lengthSquare());
}

MVec3 DropcalculateCore::generateCompositionForces(const MVec3& _speed,float _height)
{
    MVec3 resistance = generateResistance(_speed,_height);
    return MVec3(resistance.getX(),resistance.getY(),generateGravity().getZ() + generateLift(_speed,_height).getZ());
}

MVec3 DropcalculateCore::generateAccelerator(const MVec3 &_compositionForce)
{
    return _compositionForce/inner_weight;
}

MVec3 DropcalculateCore::calculteVelocity(MVec3 _oldSpeed, MVec3 _accelerator)
{
    return _oldSpeed + _accelerator;
}

MVec3 DropcalculateCore::calculatePosition(MVec3 _oldSpeed, MVec3 _oldPosition)
{
    return _oldPosition + _oldSpeed;
}

MVec3 DropcalculateCore::generateResistance(const MVec3& _speed, float _height)
{
    float tt = 0.5 * 0.08 * calcalateAirDensity(_height);
    float x = 0.5 * 0.08 * calcalateAirDensity(_height) * _speed.getX() * _speed.getX();
    float y = 0.5 * 0.08 * calcalateAirDensity(_height) * _speed.getY() * _speed.getY();
    return MVec3(x,y,0);
}

