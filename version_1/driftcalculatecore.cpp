#include "driftcalculatecore.h"

/*
 * brief: the construction function of Calculate_Core
 */
DriftCalculateCore::DriftCalculateCore()
{

}

/*
 * brief: the getter of wind vector
 *
 * output: return the wind vector,it's the direction of the wind
 */
MVec DriftCalculateCore::getInner_windVec() const
{
    return inner_windVec;
}

/*
 * brief: the setter of wind vector
 *
 * input: it need a input of MVec, which is the direction vector of wind
 */
void DriftCalculateCore::setInner_windVec(const MVec &value)
{
    inner_windVec = value;
}

/*
 * brief: the getter of current vector
 *
 * output: return the current vector,it's the direction of the current
 */
MVec DriftCalculateCore::getInner_currentVec() const
{
    return inner_currentVec;
}

/*
 * brief: the setter of current vector
 *
 * input: it need a input of MVec, which is the direction vector of current
 */
void DriftCalculateCore::setInner_currentVec(const MVec &value)
{
    inner_currentVec = value;
}

/*
 * brief: the getter of wind speed
 *
 * output: it will return the value of the speed of the wind
 */
float DriftCalculateCore::getInner_windSpeed() const
{
    return inner_windSpeed;
}

/*
 * brief: the setter of the wind speed
 *
 * input: it need a input of float value, which is the speed of the wind
 */
void DriftCalculateCore::setInner_windSpeed(float value)
{
    inner_windSpeed = value;
}

/*
 * brief: the getter of the current speed
 *
 * output: it will return the value of speed of the current
 */
float DriftCalculateCore::getInner_currentSpeed() const
{
    return inner_currentSpeed;
}

/*
 * brief: the setter of the current speed
 *
 * input: it need a input of float value, which is the speed of the current
 */
void DriftCalculateCore::setInner_currentSpeed(float value)
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
float DriftCalculateCore::getInner_updateFrequency() const
{
    return inner_updateFrequency;
}

/*
 * brief: the setter of the update requency
 *
 * input: it need a input of float value, which is the frequency of updating the parameters
 */
void DriftCalculateCore::setInner_updateFrequency(float value)
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
 * brief: the getter of the initial velocity of the object
 *
 * output: it will return the value of the velocity of the object
 */
float DriftCalculateCore::getInner_initSpeed() const
{
    return inner_initSpeed;
}

/*
 * brief: the setter of the initial velocity of the object
 *
 * input: it need a input of float value, which is the initial velocity of the object
 */
void DriftCalculateCore::setInner_initSpeed(float value)
{
    inner_initSpeed = value;
}

/*
 * brief: the setter of the spacial correlation
 *
 * input: it need a input of float value, which is the correlation of space
 */
void DriftCalculateCore::setInner_spacialCorrelation(float value)
{
    inner_SpacialCorrelation = value;
}

