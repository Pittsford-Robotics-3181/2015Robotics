/*
 * ButtonMonitor.h
 *
 *  Created on: Jan 17, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_BUTTONMONITOR_H_
#define SRC_BUTTONMONITOR_H_
#include <WPILib.h>


class ButtonResponder {
public:
	virtual void respondToButton(int button)=0;
	virtual ~ButtonResponder(){};
};

class ButtonMonitor {
public:
	ButtonMonitor(Joystick* stick, int button, ButtonResponder* responder);
	~ButtonMonitor();

	void checkButtonState();
private:
	Joystick* stick;
	int button;

	ButtonResponder* responder;

	bool oldButtonState=false;

	Notifier* monitorLoop;
};

#endif /* SRC_BUTTONMONITOR_H_ */
