#include "driftor.h"

Driftor::Driftor()
{

}

MVec2 Driftor::getInner_windSpeed() const
{
    return inner_windSpeed;
}

void Driftor::setInner_windSpeed(const MVec2 &value)
{
    inner_windSpeed = value;
}

MVec2 Driftor::getInner_currentSpeed() const
{
    return inner_currentSpeed;
}

void Driftor::setInner_currentSpeed(const MVec2 &value)
{
    inner_currentSpeed = value;
}

MVec2 Driftor::getInner_position() const
{
    return inner_position;
}

void Driftor::setInner_position(const MVec2 &value)
{
    inner_position = value;
}

