/*
 * ControlScheme.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#include "ControlScheme.h"
#include <mutex>

const int REFRENCE_FRAME_BUTTON = 11;


ControlScheme::ControlScheme(Joystick* drive, Joystick* lift) {
	driveStick = drive;
	liftStick = lift;
	referenceFrameSwitchMonitor = new ButtonMonitor(drive,REFRENCE_FRAME_BUTTON,this);
}

ControlScheme::~ControlScheme() {
	delete referenceFrameSwitchMonitor;
}

void ControlScheme::getDriveControls(double& x, double& y, double&r){
	if (getPerfectControls(x,y)){
		r = 0;
	} else {
		x = driveStick->GetX();
		y = driveStick->GetY();
		r = -driveStick->GetTwist();
	}
	//Throttle
	double throttle = (driveStick->GetThrottle() + 1)/2;
	x *= throttle;
	y *= throttle;
	r *= throttle;
}

bool ControlScheme::getPerfectControls(double& x, double& y) {
	x = driveStick->GetRawAxis(5);
	y = driveStick->GetRawAxis(6);
	return x != 0 || y != 0;
}

void ControlScheme::getLiftControls(double& vs){
	if(liftStick->GetRawButton(2)&&liftStick->GetRawButton(3)){
		vs = 0;
	} else if(liftStick->GetRawButton(3)){
		vs = 1;
	} else if(liftStick->GetRawButton(2)){
		vs = -1;
	} else {
		vs = liftStick->GetY();
	}
}

ControlReferenceFrame ControlScheme::getDriveReferenceFrame(){
	ControlReferenceFrame retFrame;
	driveRefLock.lock();
	retFrame = driveReferenceFrame;
	driveRefLock.unlock();
	return retFrame;
}

void ControlScheme::respondToButton(int button){
	if (button == REFRENCE_FRAME_BUTTON){
		driveRefLock.lock();
		switch(driveReferenceFrame){
		case ControlReferenceFrame::Absolute: driveReferenceFrame = ControlReferenceFrame::Relative; break;
		case ControlReferenceFrame::Relative: driveReferenceFrame = ControlReferenceFrame::Absolute; break;
		}
		driveRefLock.unlock();
	}
}
