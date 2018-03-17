#ifndef DRIFTOR_H
#define DRIFTOR_H

////////////////
// headers
#include <iostream>
#include <vector>
#include <math.h>

#include "all_include.h"

using namespace std;
////////////////

class Driftor
{
public:
    Driftor();

    MVec2 getInner_windSpeed() const;
    void setInner_windSpeed(const MVec2 &value);

    MVec2 getInner_currentSpeed() const;
    void setInner_currentSpeed(const MVec2 &value);

    MVec2 getInner_position() const;
    void setInner_position(const MVec2 &value);

private:
    MVec2 inner_windSpeed;
    MVec2 inner_currentSpeed;
    MVec2 inner_position;
};

#endif // DRIFTOR_H
