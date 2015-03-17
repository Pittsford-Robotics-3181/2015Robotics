#include "WPILib.h"

using namespace std;

enum State
{
	DriveToTote,
	SetUpLift,
	LowerLift,
	GrabTote,
	LiftTote,
	DriveToZone,
	DropTote,
	StopRobot

};

class Robot : public IterativeRobot
{
	private:
		bool liftState;
		
		double x, y, rotation, throttle;
		
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

		Timer*					timer;
		State					state;

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
			robotDrive->SetSafetyEnabled(false);
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

			timer 			= new Timer();


			CameraServer::GetInstance()->SetQuality(50);
			CameraServer::GetInstance()->StartAutomaticCapture("cam1");
			
		}

		void AutonomousInit()
		{

			timer->Reset();
			timer->Start();
			state = DriveToTote;
		}

		void AutonomousPeriodic()
		{

			if(state == DriveToTote){
				robotDrive->MecanumDrive_Cartesian(0,0.6,0,0);
				if(timer->Get()> 5){
					robotDrive->MecanumDrive_Cartesian(0,0,0,0);
					state = SetUpLift;
				}
			}
			else if(state == SetUpLift){
				if(){}
//				if(encoder->Get() < con)
//				{
//					liftMotor->Set(min(max((1 * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - 0.0f)/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
//				}
				else
				{
					liftState = false;
					leftLiftServo->Set(90 * liftState);
					rightLiftServo->Set(90 * !liftState);
					liftMotor->Set(0);
					state == GrabTote;
				}
			}
			else if(state == LowerLift)
			{
				if(){}
				//if(encoder->Get() < con){
//				{
//					liftMotor->Set(min(max((-1 * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - 0.0f)/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				else
				{
					liftMotor->Set(0);
					state = GrabTote;
				}
			}
			else if(state == GrabTote)
			{
				liftState = true;
				leftLiftServo->Set(90 * liftState);
				rightLiftServo->Set(90 * !liftState);
				state == LiftTote;
			}

			else if(state == LiftTote)
			{
				if(){}
			//if(encoder->Get() < con){
			//				{
			//					liftMotor->Set(min(max((-1 * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - 0.0f)/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				else
				{
					liftMotor->Set(0);
					timer->Reset();
					state = DriveToZone;
				}
			}
			else if(state == DriveToZone)
			{
				robotDrive->MecanumDrive_Cartesian(0,0.6,0,0);
				if(timer->Get()> 5){
					robotDrive->MecanumDrive_Cartesian(0,0,0,0);
					state == DropTote;
				}
			}
			else if(state == DropTote)
			{
				if(){}
				//if(encoder->Get() > con){
				//				{
				//					liftMotor->Set(min(max((-1 * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - 0.0f)/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				else
				{
					liftMotor->Set(0);
					liftState = false;
					leftLiftServo->Set(90 * liftState);
					rightLiftServo->Set(90 * !liftState);
					state == LiftTote;
				}

			}

		}

		void TeleopInit()
		{
		}

		void TeleopPeriodic()
		{
			throttle = (1.0f - rightStick->GetThrottle())/2.0f;
			if(rightStick->GetPOV() != -1 || rightStick->GetRawButton(5) || rightStick->GetRawButton(6))
			{
				x = sin(rightStick->GetPOV() * 3.14159265f/180.0f);
				y = -cos(rightStick->GetPOV() * 3.14159265f/180.0f);
				rotation = rightStick->GetRawButton(6) - rightStick->GetRawButton(5);
			}
			else
			{
				x = rightStick->GetX();
				y = rightStick->GetY();
				rotation = rightStick->GetTwist();
			}
			robotDrive->MecanumDrive_Cartesian(x * throttle, y * throttle, rotation * throttle, 0.0f);
			liftMotor->Set(min(max((leftStick->GetY() * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - leftStick->GetThrottle())/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
			liftState |= leftStick->GetRawButton(6);
			liftState &= !leftStick->GetRawButton(4);
			leftLiftServo->Set(90 * liftState);
			rightLiftServo->Set(90 * !liftState);

			SmartDashboard::PutBoolean("Lift Flap", liftState);
		}

		void TestPeriodic()
		{
			lw->Run();
		}
};

START_ROBOT_CLASS(Robot);
