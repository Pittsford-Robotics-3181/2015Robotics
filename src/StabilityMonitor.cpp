/*
 * StablityMonitor.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#include "StabilityMonitor.h"




StabilityMonitor::StabilityMonitor() {

}

StabilityMonitor::~StabilityMonitor() {
}

void StabilityMonitor::stabilizeDriveControls(double& x, double& y, double&r){

}
void StabilityMonitor::stabilizeLiftControls(double& vs){

}

class JerkLimiter {
private:
	double prevControl1=0, prevControl2=0;
public:
	double maximumJerk;
	void limitJerk(double& control){
		double jerk = control - 2*prevControl1 + prevControl2;
		if (fabs(jerk) > maximumJerk){
			jerk *= maximumJerk/fabs(jerk);
			control = jerk + 2*prevControl1 - prevControl2;
		}
		prevControl2 = prevControl1;
		prevControl1 = control;
	}
};
