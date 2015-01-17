/*
 * ButtonMonitor.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: robbiemarkwick
 */

#include "ButtonMonitor.h"
#include <WPILib.h>
#include <thread>

void checkButtonMonitor(void* m) {
	((ButtonMonitor*)m)->checkButtonState();
}

ButtonMonitor::ButtonMonitor(Joystick* stick,int button,ButtonResponder* responder) {
	this->stick=stick;
	this->button=button;
	this->responder=responder;
	monitorLoop = new Notifier(&checkButtonMonitor,this);
	monitorLoop->StartPeriodic(0.1);
}

ButtonMonitor::~ButtonMonitor(){
	monitorLoop->Stop();
	delete monitorLoop;
}

void ButtonMonitor::checkButtonState(){
	bool newButtonState = stick->GetRawButton(button);
	if (newButtonState && !oldButtonState){
		responder->respondToButton(button);
	}
	oldButtonState = newButtonState;
}
