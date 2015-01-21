/*
 * ControlScheme.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#include "ControlScheme.h"
#include <mutex>

const int REFRENCE_FRAME_BUTTON = 11;


ControlScheme::ControlScheme(Joystick* drive) {
	driveStick = drive;
	referenceFrameSwitchMonitor = new ButtonMonitor(drive,REFRENCE_FRAME_BUTTON,this);
}

ControlScheme::~ControlScheme() {
	delete referenceFrameSwitchMonitor;
}

/**
 * TODO: Read X, Y, Twist, and apply Throttle
 */
void ControlScheme::getDriveControls(double& x, double& y, double&r){

}
/**
 * TODO: Read Absolute controls
 */
bool getAbsoluteControls(double& x, double& y) {

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
