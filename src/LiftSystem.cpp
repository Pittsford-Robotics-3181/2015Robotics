/*
 * LiftSystem.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: robbiemarkwick
 */

#include <LiftSystem.h>
#include <Hardware.h>

const float highAngle = 90.0;
const float lowAngle = 0.0;
const float maxCurrent = 4;
const float maxCurrentMult = 1.2;

LiftSystem::LiftSystem(SpeedController* motor,Encoder* encoder,DigitalInput* upperProx,DigitalInput* lowerProx,Servo* left, Servo* right, PowerDistributionPanel* pdp) {
	liftMotor = motor;
	liftEncoder = encoder;
	liftEncoder->Reset();
	liftEncoder->SetDistancePerPulse(0);
	upperLimit = upperProx;
	lowerLimit = lowerProx;
	leftFlap = left;
	rightFlap = right;
	m_pdp = pdp;
	breakDown = false;

}
double LiftSystem::moveLift(double vs){
	stability->stabilizeLiftControls(vs);
	if(!breakDown || vs > 0){

		shiftAndAdd(Hardware::powerDistributionChannelLiftMotor);

		if(!isCurrentsBroken()){
			if (vs > 0 && !upperLimit->Get()){
				vs = 0;
				breakDown = false;
			}
			if (vs < 0 && !lowerLimit->Get()){
				vs = 0;
			}
			liftMotor->Set(vs);

			if(maxCurrent < m_pdp->GetCurrent(Hardware::powerDistributionChannelLiftMotor) && vs < 0){
					vs = 0;
					breakDown= true;
			}
			return vs;
		}
	}
}

bool LiftSystem::isCurrentsFull(){
	for(int i = 0; i < 4; i++ ){
		if(currents[i] == 0){
			return false;
		}
	}
	return true;
}
void LiftSystem::shiftAndAdd(double n){
	for(int i = 1; i< 4; i++){
			currents[i] = currents[i-1];
	}
	currents[0] = n; //Shifting old values Right
}

bool LiftSystem::isCurrentsBroken(){
	if(isCurrentsFull()){
		if(currents[0] * maxCurrentMult > (currents[1]+currents[2]+currents[3])/3){
			return true;
		}
		return false;
	}
	return false;
}


void LiftSystem::moveToHeight(double targetHeight, double speedScale){
	double currentHeight = liftEncoder->GetDistance();
	double speed = speedScale * 0.25 * pow(targetHeight-currentHeight,3);
	moveLift(speed);
}
void LiftSystem::moveFlapsUp(){
	leftFlap->SetAngle(highAngle);
	rightFlap->SetAngle(highAngle);
}
void LiftSystem::moveFlapsDown(){
	leftFlap->SetAngle(lowAngle);
	rightFlap->SetAngle(lowAngle);
}
