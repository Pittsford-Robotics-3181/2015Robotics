/*
 * AlignmentSystem.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: robbiemarkwick
 */

#include "AlignmentGuide.h"

const double kP = 1;
const double kI = 0;
const double kD = 0;

AlignmentGuide::AlignmentGuide(Ultrasonic *leftSensor, Ultrasonic *rightSensor)
{
    ls = leftSensor;
    rs = rightSensor;
    pidLoop = new PIDController(kP, kI, kD, this, this);
}
void AlignmentGuide::enable() { pidLoop->Enable(); }
void AlignmentGuide::disable()
{
    pidLoop->Reset();
    pidLoop->Disable();
}
double AlignmentGuide::PIDGet()
{
    return rs->GetRangeInches() - ls->GetRangeInches();
}
void AlignmentGuide::PIDWrite(float output)
{
    adjustmentLock.lock();
    rotationSpeed = output;
    adjustmentLock.unlock();
}
double AlignmentGuide::getRotationSpeed()
{
    double ret = 0;
    adjustmentLock.unlock();
    ret = rotationSpeed;
    adjustmentLock.unlock();
    return ret;
}
