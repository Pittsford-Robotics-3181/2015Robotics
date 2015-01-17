/*
 * DriveSystem.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#include "DriveSystem.h"

DriveSystem::DriveSystem(SpeedController* fl, SpeedController* fr, SpeedController* bl, SpeedController* br, Gyro* gyro) {
	frontLeft = fl;
	frontRight = fr;
	backLeft = bl;
	backRight = br;
	rotationGyro = gyro;
	stability = new StabilityMonitor();
}

DriveSystem::~DriveSystem() {

}

void DriveSystem::driveRobot(double x, double y, double r,ControlReferenceFrame referenceFrame){
	if (referenceFrame == ControlReferenceFrame::Absolute){
		double angle = readGyro();
		rotateDriveFrame(x,y,r,angle);
	}
	stability->stabilizeDriveControls(x,y,r);
	adjustMotors(x,y,r);
}

/**
 * TODO: Read Gyro in RADIANS
 */
double DriveSystem::readGyro(){
	return 0;
}
/**
 * TODO: Rotate Reference Frame
 */
void DriveSystem::rotateDriveFrame(double& x, double& y, double& r,double angle){

}
/**
 * TODO: Calculate and Set motor speeds
 */
void DriveSystem::adjustMotors(double x, double y, double r){

}















