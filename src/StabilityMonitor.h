/*
 * StablityMonitor.h
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_STABILITYMONITOR_H_
#define SRC_STABILITYMONITOR_H_

class JerkLimiter;

class StabilityMonitor {
public:
	StabilityMonitor();
	virtual ~StabilityMonitor();

	void stabilizeDriveControls(double& x, double& y, double&r);
	void stabilizeLiftControls(double& vs);
private:

};

#endif /* SRC_STABILITYMONITOR_H_ */
