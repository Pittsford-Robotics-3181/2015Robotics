/*
 * DriveSystem.h
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_DRIVESYSTEM_H_
#define SRC_DRIVESYSTEM_H_
#include "WPILib.h"
#include "StabilityMonitor.h"
#include "ControlScheme.h"

class DriveSystem {
public:
	DriveSystem(SpeedController* fl, SpeedController* fr, SpeedController* bl, SpeedController* br, Gyro* gyro);
	virtual ~DriveSystem();

	void driveRobot(double x, double y, double r,ControlReferenceFrame frame);

	StabilityMonitor* stability;

private:
	Gyro* rotationGyro;
	RobotDrive* driveMotors;

	//TO BE IMPLEMENTED
	double readGyro();
	void rotateDriveFrame(double& x, double& y, double& r,double angle);

};

#endif /* SRC_DRIVESYSTEM_H_ */
