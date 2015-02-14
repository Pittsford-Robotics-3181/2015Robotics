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
class LiftSystem {
public:
	LiftSystem(SpeedController* motor,Encoder* encoder,DigitalInput* upperSwitch,DigitalInput* lowerSwitch,SpeedController* flap,DigitalInput*flapUpper,DigitalInput*flapLower);

	void moveLift(double vs);

	void moveToHeight(double height, double speedScale = 1.0);

	void moveFlapsUp();
	void moveFlapsDown();

	StabilityMonitor* stability;

private:
	SpeedController* liftMotor;
	Encoder* liftEncoder;
	DigitalInput* upperLimit;
	DigitalInput* lowerLimit;

	SpeedController* flapMotor;
	DigitalInput* flapUpperLimit;
	DigitalInput* flapLowerLimit;
};

#endif /* SRC_LIFTSYSTEM_H_ */
