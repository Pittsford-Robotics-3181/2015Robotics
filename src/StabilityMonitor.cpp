/*
 * StablityMonitor.cpp
 *
 *	Created on: Jan 15, 2015
 *			Author: robbiemarkwick
 */

#include "StabilityMonitor.h"
#include <math.h>
#include "Calibration.h"

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
void MotionCompensator::compensateControl(double &control, double sensorVal)
{
	double motionOffset = sensorVal * controlToSensorRatio;
	SmartDashboard::PutNumber("Sensor Value", motionOffset);
	SmartDashboard::PutNumber("Control Value", control);

	if (fabs(motionOffset) > fabs(control) || control / motionOffset < 0)
	{
		motionOffset -= prevControl;
		motionOffset -= tolerance * fabs(motionOffset) / motionOffset;
		control -= motionOffset;
	}
	prevControl = control;
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
	double rot = r;
	rotationComp.compensateControl(rot, rotationGyro->GetRate());
	if (rotationCompensationEnabledState)
	{
		r = rot;
	}
}
void StabilityMonitor::stabilizeLiftControls(double &vs)
{
	jerkLift.limitJerk(vs);
	vs *= (0.8 + 0.2 * cos(++liftTime / 4));
}
