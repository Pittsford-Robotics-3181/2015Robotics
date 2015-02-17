/*
 * LiftSystem.h
 *
 *  Created on: Jan 21, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_LIFTSYSTEM_H_
#define SRC_LIFTSYSTEM_H_
#include <WPILib.h>
#include "StabilityMonitor.h"
#include "Hardware.h"
class LiftSystem {
public:
	LiftSystem(SpeedController* motor,
			   Encoder* encoder,
			   DigitalInput* upperSwitch,
			   DigitalInput* lowerSwitch,
			   Servo* left,
			   Servo* right);

	void moveLift(double vs);

	void moveToHeight(double height, double speedScale = 1.0);

	void moveFlapsUp();
	void moveFlapsDown();

	StabilityMonitor* stability;
	Encoder* liftEncoder;

private:
	SpeedController* liftMotor;
	DigitalInput* upperLimit;
	DigitalInput* lowerLimit;

	Servo* leftFlap;
	Servo* rightFlap;

};

#endif /* SRC_LIFTSYSTEM_H_ */
