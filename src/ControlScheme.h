/*
 * ControlScheme.h
 *
 *	Created on: Jan 15, 2015
 *			Author: robbiemarkwick
 */

#ifndef SRC_CONTROLSCHEME_H_
#define SRC_CONTROLSCHEME_H_
#include "WPILib.h"

enum class ControlReferenceFrame
{
	Absolute,
	Relative
};
enum class ControlAlignmentMode
{
	Align,
	Carry,
	Drive
};
class ControlScheme
{
public:
	ControlScheme(Joystick *drive, Joystick *lift);

	void getDriveControls(double &x, double &y, double &r);
	void getLiftControls(double &vs, double &liftHeight, bool &flapsUp);
	bool isRotationCompensationDisabled();
	ControlReferenceFrame getDriveReferenceFrame();
	ControlAlignmentMode getAlignmentMode();

private:
	bool getPerfectControls(double &x, double &y, double &r);
	bool rotationCompensationEnabledState = false;
	bool flapsUpState = false;
	double presetHeight = -1;

	Joystick *driveStick;
	Joystick *liftStick;

	// Reference Frame Monitor
	ControlReferenceFrame driveReferenceFrame = ControlReferenceFrame::Absolute;
};

#endif /* SRC_CONTROLSCHEME_H_ */
