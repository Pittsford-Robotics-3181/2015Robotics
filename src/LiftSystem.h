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
	LiftSystem(SpeedController* motor);

	void moveLift(double vs);

	StabilityMonitor* stability;

private:
	SpeedController* liftMotor;
};

#endif /* SRC_LIFTSYSTEM_H_ */
