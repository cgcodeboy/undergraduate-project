#ifndef DROPCALCULATECORE_H
#define DROPCALCULATECORE_H

////////////////
// headers
#include <iostream>
#include <vector>
#include <math.h>

#include <osg/AnimationPath>
////////////////


class DropcalculateCore
{
public:
    DropcalculateCore();

    osg::ref_ptr<osg::AnimationPath> calculateDropPath();

private:
    int inner_flyHeight;
    int inner_flySpeed;
};

#endif // DROPCALCULATECORE_H
