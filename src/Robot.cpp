#include "WPILib.h"
#include "ControlScheme.h"
#include "DriveSystem.h"
#include "LiftSystem.h"
#include "AlignmentGuide.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow* lw;
	DriveSystem* drive;
	ControlScheme* controls;
	LiftSystem* lift;
	AlignmentGuide* alignment;
	Timer* autoTimer;
	Ultrasonic* sonar;
	// Object for dealing with the Power Distribution Panel (PDP).
	PowerDistributionPanel m_pdp;
	USBCamera* cam = new USBCamera ("camera", true);
	// Update every 5milliseconds/0.005 seconds.
	const double kUpdatePeriod = 0.005;

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		//Drive System
		SpeedController* fl = new CANTalon(1);
		SpeedController* fr = new CANTalon(2);
		SpeedController* bl = new CANTalon(4);
		SpeedController* br = new CANTalon(3);
		Gyro* driveGyro = new Gyro(0);
		drive = new DriveSystem(fl,fr,bl,br,driveGyro);

		//Lift System
		SpeedController* lm = new CANTalon(5);
		Encoder* le = new Encoder((uint32_t)0,(uint32_t)1);
		DigitalInput *uls = new DigitalInput(3);
		DigitalInput *lls = new DigitalInput(2);
		lift = new LiftSystem(lm,le,uls,lls);

		//Control Scheme
		Joystick* driveStick = new Joystick(0);
		Joystick* liftStick = new Joystick(1);
		controls = new ControlScheme(driveStick,liftStick);

		//Stability Monitor
		StabilityMonitor* stability = new StabilityMonitor();
		stability->rotationGyro = driveGyro;
		//stability->rollGyro = new Gyro(1);
		//stability->pitchGyro = new Gyro(2);
		drive->stability = stability;
		lift->stability = stability;

		//Alignment Guide
		Ultrasonic* leftUS = new Ultrasonic((uint32_t)0,(uint32_t)0);
		Ultrasonic* rightUS = new Ultrasonic((uint32_t)0,(uint32_t)0);
		alignment = new AlignmentGuide(leftUS,rightUS);

		sonar = new Ultrasonic(5,6);


     	//Autonomous
     	autoTimer = new Timer();

	}

	void AutonomousInit()
	{
		autoTimer->Reset();
		autoTimer->Start();
	}

	void AutonomousPeriodic()
	{
		if(autoTimer->Get() > 5) {
			autoTimer->Stop();
			drive->driveRobot(0,0,0,ControlReferenceFrame::Absolute);
		} else {
			drive->driveRobot(0,-1,0,ControlReferenceFrame::Absolute);
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		//Drive
		double x=0,y=0,r=0;
		switch(controls->getAlignmentMode()){
		case ControlAlignmentMode::Align:
			alignment->enable();
			r = alignment->getRotationSpeed();
			break;
		case ControlAlignmentMode::Drive:
			alignment->disable();
			controls->getDriveControls(x,y,r);
			break;
		}
		ControlReferenceFrame referenceFrame = controls->getDriveReferenceFrame();
		bool rotationComp = controls->isRotationCompensationDisabled();
		drive->driveRobot(x,y,r,referenceFrame,rotationComp);
		//Lift
		double vs=0;
		controls->getLiftControls(vs);
		lift->moveLift(vs);

		//PDP and Carmera
				SmartDashboard::PutNumber("X", x);
				SmartDashboard::PutNumber("Y", y);
				SmartDashboard::PutNumber("R", r);

				SmartDashboard::PutNumber("sonar",sonar->GetRangeInches());

				SmartDashboard::PutNumber("Lift Motor", m_pdp.GetCurrent(3));
				// Get the current going through channel 7, in Amperes.
				// The PDP returns the current in increments of 0.125A.
				// At low currents the current readings tend to be less accurate.
				SmartDashboard::PutNumber("Front Left 13", m_pdp.GetCurrent(13));
				SmartDashboard::PutNumber("Back Left 12", m_pdp.GetCurrent(12));
				SmartDashboard::PutNumber("Front Right 2", m_pdp.GetCurrent(2));
				SmartDashboard::PutNumber("Back Right 3", m_pdp.GetCurrent(3));
				// Get the voltage going into the PDP, in Volts.
				// The PDP returns the voltage in increments of 0.05 Volts.
				SmartDashboard::PutNumber("Voltage", m_pdp.GetVoltage());
				// Retrieves the temperature of the PDP, in degrees Celsius.
				SmartDashboard::PutNumber("Temperature", m_pdp.GetTemperature());


	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
