/*
 * LiftSystem.h
 *
 *  Created on: Jan 21, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_LIFTSYSTEM_H_
#define SRC_LIFTSYSTEM_H_

class LiftSystem {
public:
	LiftSystem(SpeedController* motor);

	void moveLift(double vs);

private:
	SpeedController* liftMotor;
	StabilityMonitor* stability;
};

#endif /* SRC_LIFTSYSTEM_H_ */
