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
#include <random>

#include <driftor.h>
#include <datapipe.h>
using namespace std;
////////////////

enum COMPUTE_TYPE{
    SIMPLE,
    COMPLEX
};

enum VELOCITY_TYPE{
    WIND,
    CURRENT
};

class DriftCalculateCore
{
public:
    // the construct function
    DriftCalculateCore(COMPUTE_TYPE type);

    // the getter and setter of inner_windSpeed
    MVec getInner_windSpeed() const;
    void setInner_windSpeed(MVec value);

    // the getter and setter of inner_currentSpeed
    MVec getInner_currentSpeed() const;
    void setInner_currentSpeed(MVec value);

    // the getter and setter of inner_leewayRate
    float getInner_leewayRate() const;
    void setInner_leewayRate(float value);

    // the getter and setter of inner_updateFrequency
    int getInner_updateFrequency() const;
    void setInner_updateFrequency(int value);

    // the getter and setter of inner_initPosition
    MVec getInner_initPosition() const;
    void setInner_initPosition(const MVec &value);

    // the getter and setter of inner_initSpeed
    MVec getInner_initSpeed() const;
    void setInner_initSpeed(MVec value);

    // the setter of inner_SpacialCorrelation
    void setInner_spacialCorrelation(float value);

    //the getter of route
    vector<MVec> getDriftRoute();
    vector<MVec> getSimpleDriftRoute();
    vector<MVec> getComplexDriftRoute();

    //the setter of driftor
    void setInnder_driftorVec(const vector<Driftor> &value);

    //the setter of core driftor
    void setInner_coreDriftor(const Driftor &value);

    void setDataPipe(const DataPipe &pipe);

    void setInner_simulateTime(int value);

    void setInner_windDeviation(float value);

    void setInner_currentDeviation(float value);

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

    MVec inner_initPosition;

    MVec inner_windSpeed;
    MVec inner_currentSpeed;

    float inner_windDeviation;
    float inner_currentDeviation;

    float inner_leewayRate;

    int inner_updateFrequency;

    float inner_spacialCorrelation;

    int inner_simulateTime;

    vector<Driftor> inner_driftorVec;
    Driftor inner_coreDriftor;

    DataPipe inner_dataPipe;

    COMPUTE_TYPE inner_type;

private:
    float compute_B_correlation(int index,VELOCITY_TYPE type);

    float compute_R_correlation(MVec speed_1,MVec speed_2);

    MVec synthesisWindSpeed(MVec wind_speed);
    MVec synthesisCurrentSpeed(MVec current_speed);
    MVec synthesisObjectSpeed();

    void updatePosition();
};

#endif // DRIFTCALCULATECORE_H
