/*
 * StablityMonitor.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#include "StabilityMonitor.h"
#include <math.h>

void JerkLimiter::limitJerk(double& control){
	double accel = control - prevControl;
	if (fabs(accel) > maximumAccel){
		accel *= maximumAccel/fabs(accel);
		control = accel + prevControl;
	}
	prevControl = control;
}
void MotionCompensator::copensateControl(double& control, double sensorVal){
	double motionOffset = sensorVal * controlToSensorRatio;
	if (fabs(motionOffset - prevControl)>tolerance){
		if (motionOffset > prevControl){
			sensorVal -= tolerance;
		} else {
			sensorVal += tolerance;
		}
		motionOffset -= prevControl;
		control -= motionOffset;
	}
	prevControl = control;
}



StabilityMonitor::StabilityMonitor() {
	jerkX.maximumAccel = 0.02;
	jerkY.maximumAccel = 0.02;
	jerkMag.maximumAccel = 0.02;
	jerkR.maximumAccel = 0.02;
	jerkLift.maximumAccel = 0.02;

	rotationComp.controlToSensorRatio = 0.002929;
	rotationComp.tolerance = 0.05;
}

StabilityMonitor::~StabilityMonitor() {
}

void StabilityMonitor::stabilizeDriveControls(double& x, double& y, double&r){
	//Limit Jerk
	jerkX.limitJerk(x);
	jerkY.limitJerk(y);
	jerkR.limitJerk(r);
	double mag0 = sqrt(x*x + y*y);
	double mag = mag0;
	jerkMag.limitJerk(mag);
	if (mag0 > 0) {
		x *= fabs(mag/mag0);
		y *= fabs(mag/mag0);
	}
	//Motion Compensation
	rotationComp.copensateControl(r,rotationGyro->GetRate());
}
void StabilityMonitor::stabilizeLiftControls(double& vs){
	jerkLift.limitJerk(vs);
}

