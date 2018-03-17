#ifndef DROPCALCULATECORE_H
#define DROPCALCULATECORE_H

////////////////
// headers
#include <iostream>
#include <vector>
#include <math.h>

#include <osg/AnimationPath>

#include <all_include.h>
using namespace std;
////////////////


class DropcalculateCore
{
public:
    DropcalculateCore();

    void setInner_weight(int value);

    void setInner_flySpeed(const MVec3 &value);

    void setInner_wingArea(float value);

    void setInner_airplaneArea(float value);

    void setInner_Position(const MVec3 &value);

    vector<MVec3> getDropRoute();

private:
    MVec3 inner_Position;
    MVec3 inner_flySpeed;
    int inner_weight;
    float inner_wingArea;
    float inner_airplaneArea;

    //some const value used for calculating the force
    const float p_0 = 101.325;
    const float T_0 = 288.15;
    const float g = 9.80665;
    const float L = 0.0065;
    const float R = 8.31447;
    const float M = 0.289644;

    //some function used for calculating some significantly value
    float calculatePressure(float _height);
    float calcalateAirDensity(float _height);
    float calculateTemperature(float _height);

    //some function used for calculating the force
    MVec3 generateGravity();
    MVec3 generateLift(const MVec3& _speed, float _height);
    MVec3 generateResistance(const MVec3& _speed, float _height);
    MVec3 generateCompositionForces(const MVec3 &_speed, float _height);

    //function used for calculting the accelerator
    MVec3 generateAccelerator(const MVec3& _compositionForce);

    //function used for calculting the speed
    MVec3 calculteVelocity(MVec3 _oldSpeed,MVec3 accelerator);

    //function used for calculting the position
    MVec3 calculatePosition(MVec3 _oldSpeed,MVec3 _oldPosition);
};

#endif // DROPCALCULATECORE_H
