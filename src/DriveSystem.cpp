/*
 * DriveSystem.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#include "DriveSystem.h"
#include <math.h>

const double kFL = 1;
const double kFR = -1;
const double kBL = 1;
const double kBR = -1;


DriveSystem::DriveSystem(SpeedController* fl, SpeedController* fr, SpeedController* bl, SpeedController* br, Gyro* gyro) {
	m_fl = fl;
	m_fr = fr;
	m_bl = bl;
	m_br = br;
	rotationGyro = gyro;
	rotationGyro->Reset();
}

DriveSystem::~DriveSystem() {

}

void DriveSystem::driveRobot(double x, double y, double r,ControlReferenceFrame referenceFrame){
	if (referenceFrame == ControlReferenceFrame::Absolute){
		double angle = readGyro();
		rotateDriveFrame(x,y,r,angle);
		//Ouptut Gyro
	}
	stability->stabilizeDriveControls(x,y,r);
	adjustMotors(x,y,r);
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
void DriveSystem::adjustMotors(double x, double y, double r){
	//Normalize
	double mag = fabs(x)+fabs(y)+fabs(r);
	if(mag > 1) {
		x /= mag;
		y /= mag;
		r /= mag;
	}
	//Set motors
	double fl =  x + y + r;
	double fr = -x + y - r;
	double bl = -x + y + r;
	double br =  x + y - r;
	m_fl->Set(kFL * fl);
	m_fr->Set(kFR * fr);
	m_bl->Set(kBL * bl);
	m_br->Set(kBR * br);
}














