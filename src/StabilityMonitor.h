/*
 * StablityMonitor.h
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_STABILITYMONITOR_H_
#define SRC_STABILITYMONITOR_H_

class JerkLimiter {
public:
	double maximumJerk;
	double maximumAccel;
	void limitJerk(double& control);
private:
	double prevControl1=0, prevControl2=0;
};


class StabilityMonitor {
public:
	StabilityMonitor();
	virtual ~StabilityMonitor();

	void stabilizeDriveControls(double& x, double& y, double&r);
	void stabilizeLiftControls(double& vs);
private:
	JerkLimiter jerkX, jerkY, jerkMag, jerkR, jerkLift;
};

#endif /* SRC_STABILITYMONITOR_H_ */
