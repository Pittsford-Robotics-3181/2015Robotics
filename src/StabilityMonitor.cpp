/*
 * StablityMonitor.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#include "StabilityMonitor.h"
#include <math.h>

void JerkLimiter::limitJerk(double& control){
	double jerk = control - 2*prevControl1 + prevControl2;
	if (fabs(jerk) > maximumJerk){
		jerk *= maximumJerk/fabs(jerk);
		control = jerk + 2*prevControl1 - prevControl2;
	}
	double accel = control - prevControl1;
	if (fabs(accel) > maximumAccel){
		accel *= maximumAccel/fabs(accel);
		control = accel + prevControl1;
	}
	prevControl2 = prevControl1;
	prevControl1 = control;
}



StabilityMonitor::StabilityMonitor() {
	jerkX.maximumJerk = 0.1;
	jerkY.maximumJerk = 0.1;
	jerkMag.maximumJerk = 0.1;
	jerkR.maximumJerk = 0.1;
	jerkLift.maximumJerk = 0.1;

	jerkX.maximumAccel = 0.1;
	jerkY.maximumAccel = 0.1;
	jerkMag.maximumAccel = 0.1;
	jerkR.maximumAccel = 0.1;
	jerkLift.maximumAccel = 0.1;
}

StabilityMonitor::~StabilityMonitor() {
}

void StabilityMonitor::stabilizeDriveControls(double& x, double& y, double&r){
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
}
void StabilityMonitor::stabilizeLiftControls(double& vs){
	jerkLift.limitJerk(vs);
}

