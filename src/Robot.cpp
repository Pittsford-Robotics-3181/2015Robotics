#include "WPILib.h"

using namespace std;

class Robot : public IterativeRobot
{
	private:
		bool liftState;
		
		LiveWindow*             lw;
		
		PowerDistributionPanel* pdp;

		CANTalon*               frontLeftMotor;
		CANTalon*               frontRightMotor;
		CANTalon*               rearRightMotor;
		CANTalon*               rearLeftMotor;
		CANTalon*               liftMotor;

		RobotDrive*             robotDrive;

		Joystick*               leftStick;
		Joystick*               rightStick;
		
		DigitalInput*           lowerLiftSensor;
		DigitalInput*           upperLiftSensor;
		
		Servo*                  leftLiftServo;
		Servo*                  rightLiftServo;
		
		USBCamera*              camera;

		void RobotInit()
		{
			liftState       = false;
		
			lw              = LiveWindow::GetInstance();
			
			pdp             = new PowerDistributionPanel();
			pdp->ClearStickyFaults();

			frontLeftMotor  = new CANTalon(1);
			frontRightMotor = new CANTalon(2);
			rearRightMotor  = new CANTalon(3);
			rearLeftMotor   = new CANTalon(4);
			liftMotor       = new CANTalon(5);

			robotDrive      = new RobotDrive(frontLeftMotor, rearLeftMotor, frontRightMotor, rearRightMotor);
			robotDrive->SetInvertedMotor(RobotDrive::kFrontLeftMotor,  0);
			robotDrive->SetInvertedMotor(RobotDrive::kFrontRightMotor, 1);
			robotDrive->SetInvertedMotor(RobotDrive::kRearRightMotor,  1);
			robotDrive->SetInvertedMotor(RobotDrive::kRearLeftMotor,   0);

			leftStick       = Joystick::GetStickForPort(0);
			rightStick      = Joystick::GetStickForPort(1);
			
			lowerLiftSensor = new DigitalInput(2);
			upperLiftSensor = new DigitalInput(3);
			
			leftLiftServo   = new Servo(0);
			rightLiftServo  = new Servo(1);
			
			camera          = new USBCamera("cam0", 1);
			CameraServer::GetInstance()->SetQuality(50);
			CameraServer::GetInstance()->StartAutomaticCapture(shared_ptr<USBCamera>(camera));
			
		}

		void AutonomousInit()
		{
		}

		void AutonomousPeriodic()
		{
		}

		void TeleopInit()
		{
		}

		void TeleopPeriodic()
		{
			robotDrive->MecanumDrive_Cartesian(rightStick->GetX(), rightStick->GetY(), rightStick->GetTwist(), 0.0f);
			liftMotor->Set(min(max(leftStick->GetY(), float(-upperLiftSensor->Get())), float(lowerLiftSensor->Get())));
			liftState = leftStick->GetRawButton(2) ? 1 : rightStick->GetRawButton(3) ? 0 : liftState;
			leftLiftServo->Set(90 * liftState);
			rightLiftServo->Set(90 * !liftState);
		}

		void TestPeriodic()
		{
			lw->Run();
		}
};

START_ROBOT_CLASS(Robot);
