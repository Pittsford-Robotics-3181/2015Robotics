/*
 * LiftSystem.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: robbiemarkwick
 */

#include <LiftSystem.h>

LiftSystem::LiftSystem(SpeedController* motor,Encoder* encoder,DigitalInput* limitSwitch) {
	liftMotor = motor;
	liftEncoder = encoder;
	liftEncoder->Reset();
	liftEncoder->SetDistancePerPulse(0);
	upperLimit = limitSwitch;
}
void LiftSystem::moveLift(double vs){
	if (vs > 0 && upperLimit->Get()){
		vs = 0;
	}
	stability->stabilizeLiftControls(vs);
	liftMotor->Set(vs);
}
void LiftSystem::moveToHeight(double targetHeight, double speedScale){
	double currentHeight = liftEncoder->GetDistance();
	double speed = speedScale * 0.25 * pow(targetHeight-currentHeight,3);
	moveLift(speed);
}
