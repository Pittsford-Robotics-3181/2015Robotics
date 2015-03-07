/*
 * ControlScheme.cpp
 *
 *	Created on: Jan 15, 2015
 *			Author: robbiemarkwick
 */

#include "ControlScheme.h"

const int ABSOLUTE_REFRENCE_FRAME_BUTTON = 11;
const int RELATIVE_REFRENCE_FRAME_BUTTON = 12;
const int ENABLE_ROTATION_COMPENSATION_BUTTON = 9;
const int DISABLE_ROTATION_COMPENSATION_BUTTON = 10;
const int PERFECT_ROTATION_POSITIVE = 5;
const int PERFECT_ROTATION_NEGATIVE = 6;

const int FLAPS_UP_BUTTON = 5;
const int FLAPS_DOWN_BUTTON = 3;

ControlScheme::ControlScheme(Joystick *drive, Joystick *lift)
{
	driveStick = drive;
	liftStick = lift;
}

void ControlScheme::getDriveControls(double &x, double &y, double &r)
{
	if (!getPerfectControls(x, y, r))
	{
		x = driveStick->GetX();
		y = driveStick->GetY();
		r = driveStick->GetTwist();
	}
	// Deadzone
	if (fabs(x) < 0.1)
		x = 0;
	if (fabs(y) < 0.1)
		y = 0;
	if (fabs(r) < 0.1)
		r = 0;

	// Throttle
	double throttle = (1 - driveStick->GetThrottle()) / 2;
	x *= throttle;
	y *= throttle;
	r *= throttle;
}

bool ControlScheme::getPerfectControls(double &x, double &y, double &r)
{
	int hat = driveStick->GetPOV();
	if (hat >= 0)
	{
		if (hat > 0 && hat < 180)
		{
			x = 1;
		}
		else if (hat > 180 && hat < 360)
		{
			x = -1;
		}
		if (hat < 90 || hat > 270)
		{
			y = -1;
		}
		else if (hat > 90 && hat < 270)
		{
			y = 1;
		}
	}
	if (driveStick->GetRawButton(PERFECT_ROTATION_NEGATIVE))
	{
		r = 1;
	}
	else if (driveStick->GetRawButton(PERFECT_ROTATION_POSITIVE))
	{
		r = -1;
	}
	else
	{
		r = 0;
	}
	return x != 0 || y != 0 || r != 0;
}

void ControlScheme::getLiftControls(double &vs, double &liftHeight,
																		bool &flapsUp)
{
	vs = liftStick->GetY();

	if (liftStick->GetRawButton(6))
	{
		presetHeight = .1;
	}
	if (liftStick->GetRawButton(7))
	{
		presetHeight = 1;
	}
	if (liftStick->GetRawButton(9))
	{
		presetHeight = 2;
	}

	if (liftStick->GetRawButton(10))
	{
		presetHeight = 0;
	}
	if (liftStick->GetRawButton(11))
	{
		presetHeight = 0;
	}
	if (fabs(vs) > 0.3)
	{
		presetHeight = -1;
	}
	liftHeight = presetHeight;

	if (liftStick->GetRawButton(FLAPS_UP_BUTTON))
	{
		flapsUpState = true;
	}
	else if (liftStick->GetRawButton(FLAPS_DOWN_BUTTON))
	{
		flapsUpState = false;
	}
	flapsUp = flapsUpState;
}

bool ControlScheme::isRotationCompensationDisabled()
{
	if (driveStick->GetRawButton(ENABLE_ROTATION_COMPENSATION_BUTTON))
	{
		rotationCompensationEnabledState = true;
	}
	else if (driveStick->GetRawButton(DISABLE_ROTATION_COMPENSATION_BUTTON))
	{
		rotationCompensationEnabledState = false;
	}
	return rotationCompensationEnabledState;
}
ControlAlignmentMode ControlScheme::getAlignmentMode()
{
	return liftStick->GetTrigger() ? ControlAlignmentMode::Carry : ControlAlignmentMode::Drive;
}
ControlReferenceFrame ControlScheme::getDriveReferenceFrame()
{
	if (driveStick->GetRawButton(ABSOLUTE_REFRENCE_FRAME_BUTTON))
	{
		driveReferenceFrame = ControlReferenceFrame::Absolute;
	}
	else if (driveStick->GetRawButton(RELATIVE_REFRENCE_FRAME_BUTTON))
	{
		driveReferenceFrame = ControlReferenceFrame::Relative;
	}
	return driveReferenceFrame;
}
