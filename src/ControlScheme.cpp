/*
 * ControlScheme.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#include "ControlScheme.h"
#include <mutex>

const int ABSOLUTE_REFRENCE_FRAME_BUTTON = 11;
const int RELATIVE_REFRENCE_FRAME_BUTTON = 12;

ControlScheme::ControlScheme(Joystick* drive, Joystick* lift) {
	driveStick = drive;
	liftStick = lift;
	absoluteButtonMonitor = new ButtonMonitor(drive,ABSOLUTE_REFRENCE_FRAME_BUTTON,this);
	relativeButtonMonitor = new ButtonMonitor(drive,RELATIVE_REFRENCE_FRAME_BUTTON,this);
}

ControlScheme::~ControlScheme() {
	delete absoluteButtonMonitor;
	delete relativeButtonMonitor;
}

void ControlScheme::getDriveControls(double& x, double& y, double&r){
	if (!getPerfectControls(x,y,r)){
		x = driveStick->GetX();
		y = -driveStick->GetY();
		r = driveStick->GetTwist();
	}
	//Deadzone
	if (fabs(x) < 0.1) x = 0;
	if (fabs(y) < 0.1) y = 0;
	if (fabs(r) < 0.1) r = 0;

	//Throttle
	double throttle = (1-driveStick->GetThrottle())/2;
	x *= throttle;
	y *= throttle;
	r *= throttle;
}

bool ControlScheme::getPerfectControls(double& x, double& y, double& r) {
	int hat = driveStick->GetPOV();
	if (hat >= 0) {
		if (hat > 0 && hat < 180) {
			x = 1;
		} else if (hat > 180 && hat  < 360) {
			x = -1;
		}
		if (hat < 90 || hat > 270) {
			y = 1;
		} else if (hat > 90 && hat < 270) {
			y = -1;
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
		vs = -liftStick->GetY();
	}
<<<<<<< HEAD
}

bool ControlScheme::isRotationCompensationDisabled(){
	if(driveStick->GetRawButton(9)){
		rotationCompensationEnabledState = false;
	}
	else if(driveStick->GetRawButton(10)){
		rotationCompensationEnabledState = true;
	}
	return rotationCompensationEnabledState;
=======
	//vs *= (1 + liftStick->GetThrottle())/2;
>>>>>>> branch 'master' of https://github.com/Pittsford-Robotics-3181/2015Robotics.git
}
ControlAlignmentMode ControlScheme::getAlignmentMode(){
	return liftStick->GetTrigger() ? ControlAlignmentMode::Align : ControlAlignmentMode::Drive;
}
ControlReferenceFrame ControlScheme::getDriveReferenceFrame(){
	ControlReferenceFrame retFrame;
	driveRefLock.lock();
	retFrame = driveReferenceFrame;
	driveRefLock.unlock();
	return retFrame;
}

void ControlScheme::respondToButton(int button){
	if (button == ABSOLUTE_REFRENCE_FRAME_BUTTON){
		driveRefLock.lock();
		driveReferenceFrame = ControlReferenceFrame::Absolute;
		driveRefLock.unlock();
	} else  if (button == RELATIVE_REFRENCE_FRAME_BUTTON) {
		driveRefLock.lock();
		driveReferenceFrame = ControlReferenceFrame::Relative;
		driveRefLock.unlock();
	}
}
