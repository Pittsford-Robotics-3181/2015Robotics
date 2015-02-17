/*
 * LiftSystem.cpp
 *
 *	Created on: Jan 21, 2015
 *			Author: robbiemarkwick
 */

#include <LiftSystem.h>
#include <Hardware.h>

const float highAngle = 90.0;
const float lowAngle = 0.0;
const float maxCurrent = 50;
const float maxCurrentMult = 1.3;

LiftSystem::LiftSystem(SpeedController *motor, Encoder *encoder,
											 DigitalInput *upperProx, DigitalInput *lowerProx,
											 Servo *left, Servo *right)
{
	liftMotor = motor;
	liftEncoder = encoder;
	liftEncoder->Reset();
	liftEncoder->SetDistancePerPulse(0);
	upperLimit = upperProx;
	lowerLimit = lowerProx;
	leftFlap = left;
	rightFlap = right;
}
void LiftSystem::moveLift(double vs)
{
	stability->stabilizeLiftControls(vs);

	if (vs > 0 && !upperLimit->Get())
	{
		vs = 0;
	}
	if (vs < 0 && !lowerLimit->Get())
	{
		vs = 0;
	}
	liftMotor->Set(vs);
}

void LiftSystem::moveToHeight(double targetHeight, double speedScale)
{
	double currentHeight = liftEncoder->GetDistance();
	double speed = speedScale * 0.25 * pow(targetHeight - currentHeight, 3);
	moveLift(speed);
}
void LiftSystem::moveFlapsUp()
{
	leftFlap->SetAngle(lowAngle);
	rightFlap->SetAngle(highAngle);
}
void LiftSystem::moveFlapsDown()
{
	leftFlap->SetAngle(highAngle);
	rightFlap->SetAngle(lowAngle);
}
