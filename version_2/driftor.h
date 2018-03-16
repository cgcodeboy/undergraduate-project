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

    MVec getInner_windSpeed() const;
    void setInner_windSpeed(const MVec &value);

    MVec getInner_currentSpeed() const;
    void setInner_currentSpeed(const MVec &value);

    MVec getInner_position() const;
    void setInner_position(const MVec &value);

private:
    MVec inner_windSpeed;
    MVec inner_currentSpeed;
    MVec inner_position;
};

#endif // DRIFTOR_H
