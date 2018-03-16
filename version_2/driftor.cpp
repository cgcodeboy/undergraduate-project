#include "driftor.h"

Driftor::Driftor()
{

}

MVec Driftor::getInner_windSpeed() const
{
    return inner_windSpeed;
}

void Driftor::setInner_windSpeed(const MVec &value)
{
    inner_windSpeed = value;
}

MVec Driftor::getInner_currentSpeed() const
{
    return inner_currentSpeed;
}

void Driftor::setInner_currentSpeed(const MVec &value)
{
    inner_currentSpeed = value;
}

MVec Driftor::getInner_position() const
{
    return inner_position;
}

void Driftor::setInner_position(const MVec &value)
{
    inner_position = value;
}

