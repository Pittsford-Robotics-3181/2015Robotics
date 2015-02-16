/*
 * ControlScheme.h
 *
 *  Created on: Jan 15, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_CONTROLSCHEME_H_
#define SRC_CONTROLSCHEME_H_
#include "WPILib.h"

enum class ControlReferenceFrame {
	Absolute,Relative
};
enum class ControlAlignmentMode {
	Align, Carry, Drive
};
class ControlScheme {
public:
	ControlScheme(Joystick* drive, Joystick* lift);

	void getDriveControls(double& x, double& y, double&r);
	void getLiftControls(double& vs, bool& flapsUp);
	bool isRotationCompensationDisabled();
	ControlReferenceFrame getDriveReferenceFrame();
	ControlAlignmentMode getAlignmentMode();

	void respondToButton(int button);

private:
	bool getPerfectControls(double& x, double& y, double& r);
	bool rotationCompensationEnabledState = true;
	bool flapsUpState = false;

	Joystick* driveStick;
	Joystick* liftStick;

	//Reference Frame Monitor
	ControlReferenceFrame driveReferenceFrame = ControlReferenceFrame::Absolute;

};

#endif /* SRC_CONTROLSCHEME_H_ */
