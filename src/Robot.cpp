#include "WPILib.h"
#include "ControlScheme.h"
#include "DriveSystem.h"
#include "LiftSystem.h"
#include "AlignmentGuide.h"
#include "Calibration.h"
#include "Hardware.h"

class Robot : public IterativeRobot
{
private:
	LiveWindow *lw;
	DriveSystem *drive;
	RobotDrive* robotDrive;
	ControlScheme *controls;
	LiftSystem *lift;
	AlignmentGuide *guide;

	Joystick *driveStick;
	Joystick *liftStick;

	Gyro *driveGyro;

	bool flapUp = false;

	DigitalInput *lls;
	DigitalInput *uls;
	AlignmentGuide *alignment;
	Timer *autoTimer;
	double liftValue = 0;
	Ultrasonic *sonarR;
	Ultrasonic *sonarL;
	// Object for dealing with the Power Distribution Panel (PDP).
	PowerDistributionPanel *m_pdp;
	//USBCamera *cam = new USBCamera("cam1", 0);
	// Update every 5milliseconds/0.005 seconds.
	const double kUpdatePeriod = 0.005;

	void RobotInit()
	{
		m_pdp = new PowerDistributionPanel();
//		lw = LiveWindow::GetInstance();
		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam1");
		// Drive System
		SpeedController *fl = new CANTalon(Hardware::frontLeftDriveMotor);
		SpeedController *fr = new CANTalon(Hardware::frontRightDriveMotor);
		SpeedController *bl = new CANTalon(Hardware::backLeftDriveMotor);
		SpeedController *br = new CANTalon(Hardware::backRightDriveMotor);
		driveGyro = new Gyro(Hardware::driveRotationGyro);

		//RobotDrive(SpeedController *frontLeftMotor, SpeedController *rearLeftMotor,
		//SpeedController *frontRightMotor, SpeedController *rearRightMotor);

		drive = new DriveSystem(fl, bl, fr, br, driveGyro);

		// Lift System
		SpeedController *lm = new CANTalon(Hardware::liftMotor);
		Encoder *le = new Encoder(Hardware::liftEncoderPort1, Hardware::liftEncoderPort2);
		uls = new DigitalInput(Hardware::liftProxUpper);
		lls = new DigitalInput(Hardware::liftProxLower);
		Servo *lfs = new Servo(Hardware::leftServo);
		Servo *rfs = new Servo(Hardware::rightServo);
		lift = new LiftSystem(lm, le, uls, lls, lfs, rfs);

		// Control Scheme
		driveStick = new Joystick(0);
		liftStick = new Joystick(1);
		controls = new ControlScheme(driveStick, liftStick);

		// Stability Monitor
		StabilityMonitor *stability = new StabilityMonitor();
		stability->rotationGyro = driveGyro;
		//drive->stability = stability;
		lift->stability = stability;

		//Alignment Guide

		sonarR = new Ultrasonic(Hardware::sonarPingR, Hardware::sonarEchoR);
		sonarL = new Ultrasonic(Hardware::sonarPingL, Hardware::sonarEchoL);
		guide = new AlignmentGuide(sonarL, sonarR);
		guide->setAutomaticSensors(true);


		// Autonomous
		autoTimer = new Timer();

		m_pdp->ClearStickyFaults();

	}

	void AutonomousInit()
	{
		autoTimer->Reset();
		autoTimer->Start();
	}

	void AutonomousPeriodic()
	{
		lift->moveLift(-0.5);
		if (autoTimer->Get() > 5)
		{
			autoTimer->Stop();
			//drive->driveRobot(0,0,0,ControlReferenceFrame::Absolute,true);
		}
		else
		{
			//drive->driveRobot(0,-1,0,ControlReferenceFrame::Absolute,true);
		}
	}

	void TeleopInit()
	{
		lift->liftEncoder->Reset();
	}

	void TeleopPeriodic()
	{
		// Drive
		double x = 0, y = 0, r = 0;
		switch (controls->getAlignmentMode())
		{
		case ControlAlignmentMode::Align:
		///	alignment->enable();
		//	r = alignment->getRotationSpeed();
		//	break;
		case ControlAlignmentMode::Carry:
			// alignment->disable();
			controls->getDriveControls(x, y, r);
			x = Calibration::CARRY_XPR * r;
			break;
		case ControlAlignmentMode::Drive:
			// alignment->disable();
			controls->getDriveControls(x, y, r);
			break;
		}

		ControlReferenceFrame referenceFrame = controls->getDriveReferenceFrame();
		bool rotationComp = controls->isRotationCompensationDisabled();
		drive->driveRobot(x, y, r, referenceFrame, rotationComp);


		// Lift
		double vs = 0, liftHeight = 0;

		controls->getLiftControls(vs, liftHeight, flapUp);
		liftValue = vs;
		if (liftHeight >= 0)
		{
			lift->moveToHeight(liftHeight);
		}
		else
		{
			lift->moveLift(vs);
		}
		if (flapUp)
		{
			lift->moveFlapsUp();
		}
		else
		{
			lift->moveFlapsDown();
		}

		printDiagnostics(x, y, r);
	}

	void printDiagnostics(double x, double y, double r)
	{
		// PDP and Carmera
		SmartDashboard::PutNumber("Joystick X", driveStick->GetX());
		SmartDashboard::PutNumber("Joystick Y", driveStick->GetY());
		SmartDashboard::PutNumber("Joystick R", driveStick->GetZ());

		SmartDashboard::PutNumber("sonarL", sonarL->GetRangeInches());
		SmartDashboard::PutNumber("sonarR", sonarR->GetRangeInches());
		SmartDashboard::PutNumber("Alignment different", guide->PIDGet());

		SmartDashboard::PutNumber("Lift Motor", m_pdp->GetCurrent(3));
		SmartDashboard::PutBoolean("upper limit switch", !uls->Get());
		SmartDashboard::PutBoolean("lower limit switch", !lls->Get());

		SmartDashboard::PutNumber("liftValue", liftValue);
		SmartDashboard::PutString("Right flap motor", flapUp ? "rotatingUp":"raotatingDown");

		// Get the current going through channel 7, in Amperes.
		// The PDP returns the current in increments of 0.125A.
		// At low currents the current readings tend to be less accurate.
		SmartDashboard::PutNumber("Front Left 15", m_pdp->GetCurrent(15));
		SmartDashboard::PutNumber("Front Right 14", m_pdp->GetCurrent(14));
		SmartDashboard::PutNumber("Back Left 12", m_pdp->GetCurrent(12));
		SmartDashboard::PutNumber("Back Right 13", m_pdp->GetCurrent(3));
		// Get the voltage going into the PDP, in Volts.
		// The PDP returns the voltage in increments of 0.05 Volts.
		SmartDashboard::PutNumber("Voltage", m_pdp->GetVoltage());
		// Retrieves the temperature of the PDP, in degrees Celsius.
		SmartDashboard::PutNumber("Temperature", m_pdp->GetTemperature());

		SmartDashboard::PutNumber("Rotation Rate", driveGyro->GetRate());
	}

//Users/public/documents/frc/FRC Data Storage, delete this to get dashboard to depl
	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
