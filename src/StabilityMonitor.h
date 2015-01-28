/*
 * StablityMonitor.h
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_STABILITYMONITOR_H_
#define SRC_STABILITYMONITOR_H_
#include <WPILib.h>

class JerkLimiter {
public:
	double maximumAccel;
	void limitJerk(double& control);
private:
	double prevControl = 0;
};

class MotionCompensator {
public:
	double controlToSensorRatio = 1, tolerance = 0;
	void copensateControl(double& control, double sensorVal);
private:
	double prevControl = 0;
};

class StabilityMonitor {
public:
	StabilityMonitor();
	virtual ~StabilityMonitor();
	Gyro* rotationGyro;//, *rollGyro, *pitchGyro;

	void stabilizeDriveControls(double& x, double& y, double&r);
	void stabilizeLiftControls(double& vs);
private:
	JerkLimiter jerkX, jerkY, jerkMag, jerkR, jerkLift;
	MotionCompensator rotationComp;
};

#endif /* SRC_STABILITYMONITOR_H_ */
