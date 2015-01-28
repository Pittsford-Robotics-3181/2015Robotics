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
	LiftSystem(SpeedController* motor,Encoder* encoder);

	void moveLift(double vs);

	void moveToHeight(double height, double speedScale = 1.0);

	StabilityMonitor* stability;

private:
	SpeedController* liftMotor;
	Encoder* liftEncoder;
};

#endif /* SRC_LIFTSYSTEM_H_ */
