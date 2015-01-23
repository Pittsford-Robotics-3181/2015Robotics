/*
 * LiftSystem.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: robbiemarkwick
 */

#include <LiftSystem.h>

LiftSystem::LiftSystem(SpeedController* motor) {
	liftMotor = motor;
}
void LiftSystem::moveLift(double vs){
	liftMotor->Set(vs);
}
