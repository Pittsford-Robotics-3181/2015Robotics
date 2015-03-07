/*
 * StablityMonitor.cpp
 *
 *	Created on: Jan 15, 2015
 *			Author: robbiemarkwick
 */

#include "StabilityMonitor.h"
#include <math.h>
#include "Calibration.h"
#include <iostream>
#include <fstream>

void JerkLimiter::limitJerk(double &control)
{
	double accel = control - prevControl;
	if (fabs(accel) > maximumAccel)
	{
		accel *= maximumAccel / fabs(accel);
		control = accel + prevControl;
	}
	prevControl = control;
}
void MotionCompensator::compensateControl(double &control, double sensorVal,bool enabled, bool recursive)
{
/*	double motionOffset = sensorVal * controlToSensorRatio;
	SmartDashboard::PutNumber("Sensor Value", motionOffset);
	SmartDashboard::PutNumber("Control Value", control);

	if (fabs(motionOffset) > fabs(control) || control / motionOffset < 0)
	{
		motionOffset -= prevControl;
		motionOffset -= tolerance * fabs(motionOffset) / motionOffset;
		if (fabs(motionOffset) > 0.7 && enabled && !recursive) {
			invertSensor();
			compensateControl(control,sensorVal,enabled,true);
		}
		if (enabled){
			control -= motionOffset;
			prevControl  = control;
		} else {
			prevControl = control - motionOffset;
		}
	} else {
		prevControl = control;
	}
	*/
}
void MotionCompensator::invertSensor(){
	bool inverted = false;

	std::ifstream inFile;
	inFile.open("/INVERSION.txt");
	if (!inFile.fail()) {
		char ch;
		inFile.get(ch);
		inFile.close();
		if (ch == 'F') {
			inverted = true;
		}
	}

	inverted = !inverted;
	controlToSensorRatio = fabs(controlToSensorRatio);
	controlToSensorRatio = inverted ? -1 : 1;

	std::ofstream outFile;
	outFile.open("/INVERSION.txt");
	if (!outFile.fail()) {
		outFile.put(inverted ? 'F' : 'T');
	}
	outFile.close();
}

StabilityMonitor::StabilityMonitor()
{
	jerkX.maximumAccel = Calibration::MAX_ACCEL_X;
	jerkY.maximumAccel = Calibration::MAX_ACCEL_Y;
	jerkMag.maximumAccel = Calibration::MAX_ACCEL_MAG;
	jerkR.maximumAccel = Calibration::MAX_ACCEL_R;
	jerkLift.maximumAccel = Calibration::MAX_ACCEL_LIFT;

	rotationComp.controlToSensorRatio =
			Calibration::ROT_COMP_CONTROL_SENSOR_RATIO;
	rotationComp.tolerance = Calibration::ROT_COMP_TOLERANCE;
}

StabilityMonitor::~StabilityMonitor()
{
}

void
StabilityMonitor::stabilizeDriveControls(double &x, double &y, double &r,
																				 bool rotationCompensationEnabledState)
{
	// Limit Jerk
	jerkX.limitJerk(x);
	jerkY.limitJerk(y);
	jerkR.limitJerk(r);
	double mag0 = sqrt(x * x + y * y);
	double mag = mag0;
	jerkMag.limitJerk(mag);
	if (mag0 > 0)
	{
		x *= fabs(mag / mag0);
		y *= fabs(mag / mag0);
	}

	// Motion Compensation
	//double sensor = rotationGyro->GetRate();
	//rotationComp.compensateControl(r, sensor,rotationCompensationEnabledState);

}
void StabilityMonitor::stabilizeLiftControls(double &vs)
{
	jerkLift.limitJerk(vs);
	vs *= (0.95 + 0.05 * cos(++liftTime / 2));
}
