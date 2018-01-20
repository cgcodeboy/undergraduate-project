/*******************************
 * brief: a class for calculating the route of driftor
 *
 *        we need input the wind speed、 wind direction、current speed、current direction
 *        for the calculating procedure, apart from these ,we alse need to input some
 *        parameters like the standard deviation of wind speed and currend speed and the
 *        leeway rate、update frequency.
 *
 * author:cgcodeboy
 *
 * time:2018-1-20
 *
 ******************************/

#ifndef DRIFTCALCULATECORE_H
#define DRIFTCALCULATECORE_H

////////////////
// headers
#include <iostream>
#include <vector>
#include <math.h>

#include "all_include.h"
////////////////


class DriftCalculateCore
{
public:
    // the construct function
    DriftCalculateCore();

    // the getter and setter of inner_windVec
    MVec getInner_windVec() const;
    void setInner_windVec(const MVec &value);

    // the getter and setter of inner_currentVec
    MVec getInner_currentVec() const;
    void setInner_currentVec(const MVec &value);

    // the getter and setter of inner_windSpeed
    float getInner_windSpeed() const;
    void setInner_windSpeed(float value);

    // the getter and setter of inner_currentSpeed
    float getInner_currentSpeed() const;
    void setInner_currentSpeed(float value);

    // the getter and setter of inner_leewayRate
    float getInner_leewayRate() const;
    void setInner_leewayRate(float value);

    // the getter and setter of inner_updateFrequency
    float getInner_updateFrequency() const;
    void setInner_updateFrequency(float value);

    // the getter and setter of inner_initPosition
    MVec getInner_initPosition() const;
    void setInner_initPosition(const MVec &value);

    // the getter and setter of inner_initSpeed
    float getInner_initSpeed() const;
    void setInner_initSpeed(float value);

    // the setter of inner_SpacialCorrelation
    void setInner_spacialCorrelation(float value);

private:
    /*
     * brief: introduction of some parameter
     *
     * windVec: the direction of the wind
     * windSpeed: the velocity of the wind
     *
     * currentVec: the direction of the current
     * currentSpeed: the velocity of the current
     *
     * initPosition: the initial position of the object
     * initSpeed: the initial velocity of the object
     *
     * leewayRate: the rate of the leeway
     *             let
     *                  F_w = 0.5 * p_a * C_a * A_a * |V_w| * V_w
     *                  F_c = 0.5 * p_w * C_w * A_w * |V_c| * V_c
     *             let
     *                  (p_a * C_a * A_a)/(p_w * C_w * A_w) = tt^2
     *             let
     *                  f = 1/(1 + tt)
     *             the f represent the leeway rate
     *
     * updateFrequency: the frequency to update the parameter
     *                  like direction and velocity of wind and current
     *
     * spacialCorrelation: current and wind fields are assumed to be described by
     *                     homogenous random fields, which means that the correlation between
     *                     two locations depends only on the distance between them and not on
     *                     their locations. spacialCorrelation is the scale of Eulerian correlation velocity,
     *                     which calibrates the degree of correlation. A larger value of spacialCorrelation
     *                     means higher spatial correlations between nearby locations.
     */
    MVec inner_windVec;
    MVec inner_currentVec;

    MVec inner_initPosition;

    float inner_initSpeed;

    float inner_windSpeed;
    float inner_currentSpeed;

    float inner_leewayRate;
    float inner_updateFrequency;

    float inner_spacialCorrelation;
};

#endif // DRIFTCALCULATECORE_H
