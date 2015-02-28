/*
 * DriveSystem.cpp
 *
 *	Created on: Jan 15, 2015
 *			Author: robbiemarkwick
 */

#include "DriveSystem.h"
#include <math.h>
#include "StabilityMonitor.h"

DriveSystem::DriveSystem(SpeedController *fl, SpeedController *bl,
												 SpeedController *fr, SpeedController *br, Gyro *gyro)
{
	rotationGyro = gyro;
	rotationGyro->Reset();

	rd = new RobotDrive(fl, bl, fr, br);
	//RobotDrive(SpeedController *frontLeftMotor, SpeedController *rearLeftMotor,
			//SpeedController *frontRightMotor, SpeedController *rearRightMotor);
	rd->SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
	rd->SetInvertedMotor(RobotDrive::kRearRightMotor, true);

	rd->SetSafetyEnabled(false);

}

DriveSystem::~DriveSystem()
{
}

void DriveSystem::driveRobot(double x, double y, double r, ControlReferenceFrame referenceFrame,bool rotationCompensationEnabledState)
{
	stability->stabilizeDriveControls(x, y, r, rotationCompensationEnabledState);


	SmartDashboard::PutNumber("Motor X", x);
	SmartDashboard::PutNumber("Motor Y", y);
	SmartDashboard::PutNumber("Motor R", r);


	switch (referenceFrame)
	{
	case ControlReferenceFrame::Absolute:
		rd->MecanumDrive_Cartesian(x, y, r,0);
		break;
	case ControlReferenceFrame::Relative:
		rd->MecanumDrive_Cartesian(x, y, r, 0);
		break;
	}
}
