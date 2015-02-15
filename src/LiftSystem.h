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
	LiftSystem(SpeedController* motor,Encoder* encoder,DigitalInput* upperSwitch,DigitalInput* lowerSwitch,Servo* left, Servo* right, PowerDistributionPanel* pdp);

	double moveLift(double vs);

	void moveToHeight(double height, double speedScale = 1.0);

	void moveFlapsUp();
	void moveFlapsDown();

	bool isCurrentsFull();
	void shiftAndAdd(double n);
	bool isCurrentsBroken();

	StabilityMonitor* stability;

private:
	SpeedController* liftMotor;
	Encoder* liftEncoder;
	DigitalInput* upperLimit;
	DigitalInput* lowerLimit;

	Servo* leftFlap;
	Servo* rightFlap;

	PowerDistributionPanel* m_pdp;

	double currents[4];

	bool breakDown;
};

#endif /* SRC_LIFTSYSTEM_H_ */
