/*
 * AlignmentSystem.h
 *
 *  Created on: Feb 3, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_ALIGNMENTGUIDE_H_
#define SRC_ALIGNMENTGUIDE_H_
#include "WPILib.h"

class AlignmentGuide: public PIDSource,public  PIDOutput {
public:
	AlignmentGuide(Ultrasonic * leftSensor,Ultrasonic * rightSensor);

	void enable();
	void disable();


	double getRotationSpeed();
	virtual double PIDGet();
	virtual void PIDWrite(float output);
private:
	Ultrasonic * ls, * rs;
	double rotationSpeed = 0.0;
	PIDController* pidLoop;
	std::mutex adjustmentLock;

};

#endif /* SRC_ALIGNMENTGUIDE_H_ */
