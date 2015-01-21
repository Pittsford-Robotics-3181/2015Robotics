/*
 * DriveSystem.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#include "DriveSystem.h"
#include <math.h>



DriveSystem::DriveSystem(SpeedController* fl, SpeedController* fr, SpeedController* bl, SpeedController* br, Gyro* gyro) {
	driveMotors = new RobotDrive(fl,bl,fr,br);
	rotationGyro = gyro;
}

DriveSystem::~DriveSystem() {

}

void DriveSystem::driveRobot(double x, double y, double r,ControlReferenceFrame referenceFrame){
	if (referenceFrame == ControlReferenceFrame::Absolute){
		double angle = readGyro();
		rotateDriveFrame(x,y,r,angle);
	}
	stability->stabilizeDriveControls(x,y,r);
	driveMotors->MecanumDrive_Cartesian(x,y,r);
}

double DriveSystem::readGyro(){
	double degrees = rotationGyro->GetAngle();
	double radians = degrees * (-M_PI/180.0);
	return radians;
}

void DriveSystem::rotateDriveFrame(double& x, double& y, double& r,double angle){
	double inX = x;
	double inY = y;
	double cosA = cos(angle);
	double sinA = sin(angle);

	x = cosA*inX - sinA*inY;
	y = sinA*inX + cosA*inY;
}















