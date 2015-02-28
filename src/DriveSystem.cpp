/*
 * DriveSystem.cpp
 *
 *	Created on: Jan 15, 2015
 *			Author: robbiemarkwick
 */

#include "DriveSystem.h"
#include <math.h>
#include "StabilityMonitor.h"

DriveSystem::DriveSystem(SpeedController *fl, SpeedController *fr,
												 SpeedController *bl, SpeedController *br, Gyro *gyro)
{

	rotationGyro = gyro;
	rotationGyro->Reset();

	rd = new RobotDrive(fl, fr, bl, br);
	rd->SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);
	rd->SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
	rd->SetSafetyEnabled(false);
}

DriveSystem::~DriveSystem()
{
}

void DriveSystem::driveRobot(double x, double y, double r,
														 ControlReferenceFrame referenceFrame,
														 bool rotationCompensationEnabledState, bool useWPI)
{
	stability->stabilizeDriveControls(x, y, r, rotationCompensationEnabledState);
	SmartDashboard::PutNumber("Motor X", x);
	SmartDashboard::PutNumber("Motor Y", y);
	SmartDashboard::PutNumber("Motor R", r);
	switch (referenceFrame)
	{
	case ControlReferenceFrame::Absolute:
		rd->MecanumDrive_Cartesian(x, y, r, rotationGyro->GetAngle());
		break;
	case ControlReferenceFrame::Relative:
		rd->MecanumDrive_Cartesian(x, y, r, 0);
		break;
	}
}
