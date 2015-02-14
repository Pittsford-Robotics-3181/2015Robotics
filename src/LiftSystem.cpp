/*
 * LiftSystem.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: robbiemarkwick
 */

#include <LiftSystem.h>

const double flapSpeed = 1.0;

LiftSystem::LiftSystem(SpeedController* motor,Encoder* encoder,DigitalInput* upperSwitch,DigitalInput* lowerSwitch,SpeedController* flap,DigitalInput*flapUpper,DigitalInput*flapLower) {
	liftMotor = motor;
	liftEncoder = encoder;
	liftEncoder->Reset();
	liftEncoder->SetDistancePerPulse(0);
	upperLimit = upperSwitch;
	lowerLimit = lowerSwitch;
	flapUpperLimit = flapUpper;
	flapLowerLimit = flapLower;
	flapMotor = flap;
}
void LiftSystem::moveLift(double vs){
	stability->stabilizeLiftControls(vs);
	if (vs > 0 && upperLimit->Get()){
		vs = 0;
	}
	if (vs < 0 && lowerLimit->Get()){
		vs = 0;
	}
	liftMotor->Set(vs);
}
void LiftSystem::moveToHeight(double targetHeight, double speedScale){
	double currentHeight = liftEncoder->GetDistance();
	double speed = speedScale * 0.25 * pow(targetHeight-currentHeight,3);
	moveLift(speed);
}
void LiftSystem::moveFlapsUp(){
	if (flapUpperLimit->Get()){
		flapMotor->Set(0);
	} else {
		flapMotor->Set(flapSpeed);
	}
}
void LiftSystem::moveFlapsDown(){
	if (flapLowerLimit->Get()){
		flapMotor->Set(0);
	} else {
		flapMotor->Set(-flapSpeed);
	}
}
