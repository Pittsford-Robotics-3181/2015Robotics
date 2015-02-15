#include "WPILib.h"
#include "ControlScheme.h"
#include "DriveSystem.h"
#include "LiftSystem.h"
#include "AlignmentGuide.h"
#include "Calibration.h"
#include "Hardware.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow* lw;
	DriveSystem* drive;
	ControlScheme* controls;
	LiftSystem* lift;

	DigitalInput *lls;
	DigitalInput *uls;
	AlignmentGuide* alignment;
	Timer* autoTimer;
	Ultrasonic* sonarR;
	Ultrasonic* sonarL;
	// Object for dealing with the Power Distribution Panel (PDP).
	PowerDistributionPanel m_pdp;
	USBCamera* cam = new USBCamera("cam1",0);
	// Update every 5milliseconds/0.005 seconds.
	const double kUpdatePeriod = 0.005;

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture(std::shared_ptr<USBCamera>(cam));
		//Drive System
		SpeedController* fl = new CANTalon(Hardware::frontLeftDriveMotor);
		SpeedController* fr = new CANTalon(Hardware::frontRightDriveMotor);
		SpeedController* bl = new CANTalon(Hardware::backLeftDriveMotor);
		SpeedController* br = new CANTalon(Hardware::backRightDriveMotor);
		Gyro* driveGyro = new Gyro(Hardware::driveRotationGyro);
		drive = new DriveSystem(fl,fr,bl,br,driveGyro);

		//Lift System
		SpeedController* lm = new CANTalon(Hardware::liftMotor);
		Encoder* le = new Encoder(Hardware::liftEncoderPort1,Hardware::liftEncoderPort2);
		uls = new DigitalInput(Hardware::liftProxUpper);
		lls = new DigitalInput(Hardware::liftProxLower);
		Servo* lfs = new Servo(Hardware::leftServo);
		Servo* rfs = new Servo(Hardware::rightServo);
		lift = new LiftSystem(lm,le,uls,lls,lfs,rfs);

		//Control Scheme
		Joystick* driveStick = new Joystick(0);
		Joystick* liftStick = new Joystick(1);
		controls = new ControlScheme(driveStick,liftStick);

		//Stability Monitor
		StabilityMonitor* stability = new StabilityMonitor();
		stability->rotationGyro = driveGyro;
		drive->stability = stability;
		lift->stability = stability;

	/*	//Alignment Guide
		Ultrasonic* leftUS = new Ultrasonic((uint32_t)0,(uint32_t)0);
		Ultrasonic* rightUS = new Ultrasonic((uint32_t)0,(uint32_t)0);
		alignment = new AlignmentGuide(leftUS,rightUS);
*/
		sonarR = new Ultrasonic(Hardware::sonarPingR,Hardware::sonarEchoR);
		sonarR->SetAutomaticMode(true);

		sonarL = new Ultrasonic(Hardware::sonarPingL,Hardware::sonarEchoL);
		sonarL->SetAutomaticMode(true);

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
		///	alignment->enable();
		//	r = alignment->getRotationSpeed();
		//	break;
		case ControlAlignmentMode::Carry:
			//alignment->disable();
			controls->getDriveControls(x,y,r);
			x = Calibration::CARRY_XPR * r;
			break;
		case ControlAlignmentMode::Drive:
			//alignment->disable();
			controls->getDriveControls(x,y,r);
			break;
		}
		ControlReferenceFrame referenceFrame = controls->getDriveReferenceFrame();
		bool rotationComp = controls->isRotationCompensationDisabled();
		drive->driveRobot(x,y,r,referenceFrame,rotationComp);
		//Lift
		double vs=0;
		bool flapUp=false;
		controls->getLiftControls(vs,flapUp);
		lift->moveLift(vs);
		if(flapUp) {
			lift->moveFlapsUp();
		} else {
			lift->moveFlapsDown();
		}

		printDiagnostics(x,y,r);
	}


	void printDiagnostics(double x, double y, double r){
		//PDP and Carmera
						SmartDashboard::PutNumber("X", x);
						SmartDashboard::PutNumber("Y", y);
						SmartDashboard::PutNumber("R", r);


						SmartDashboard::PutNumber("sonarL",sonarL->GetRangeInches());
						SmartDashboard::PutNumber("sonarR",sonarR->GetRangeInches());

						if(uls->Get()){
							SmartDashboard::PutBoolean("Tester",uls->Get());
						}
						SmartDashboard::PutNumber("Lift Motor", m_pdp.GetCurrent(3));
						SmartDashboard::PutBoolean("upper limit switch",uls->Get());
						SmartDashboard::PutBoolean("lower limit switch",lls->Get());

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
