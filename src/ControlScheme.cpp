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
	if (!getPerfectControls(x,y,r)){
		x = driveStick->GetX();
		y = driveStick->GetY();
		r = driveStick->GetTwist();
	}
	SmartDashboard::PutNumber("Y", y);
	//Throttle
	double throttle = (1-driveStick->GetThrottle())/2;
	x *= throttle;
	y *= throttle;
	r *= throttle;
}

bool ControlScheme::getPerfectControls(double& x, double& y, double& r) {
	int hat = driveStick->GetPOV();
	SmartDashboard::PutNumber("HAT", hat);
	if (hat >= 0) {
		if (hat > 0 && hat < 180) {
			x = 1;
		} else if (hat > 180 && hat  < 360) {
			x = -1;
		}
		if (hat < 90 || hat > 270) {
			y = -1;
		} else if (hat > 90 && hat < 270) {
			y = 1;
		}
	}
	if (driveStick->GetRawButton(6)){
		r = 1;
	} else if(driveStick->GetRawButton(5)){
		r = -1;
	} else {
		r = 0;
	}
	return x != 0 || y != 0 || r != 0;
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
	vs *= (1 + liftStick->GetThrottle())/2;
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
