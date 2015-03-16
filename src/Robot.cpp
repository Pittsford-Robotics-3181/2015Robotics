#include "WPILib.h"

using namespace std;

enum State
{
	DriveRobot,
	StopRobot

};

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

			state = 0;

			CameraServer::GetInstance()->SetQuality(50);
			CameraServer::GetInstance()->StartAutomaticCapture("cam1");
			
		}

		void AutonomousInit()
		{

			timer->Reset();
			timer->Start();
		}

		void AutonomousPeriodic()
		{

			if(state = )

			if(timer->Get() < 5)
			{
				if(timer->Get() < 5)
				{
					state = 1;
				}
				else if(timer->Get() > 5 && timer->Get() < 10 )
				{
					state = 2;
				}
				if(state != 2 && state < 5 && state != 0){

					robotDrive->MecanumDrive_Cartesian(0,.2,0,0);
				}

				if(state == 2){
					liftMotor->Set(min(max(0.5f*0.75f + static_cast<float>(sin(GetClock()/1000.0f))*0.25f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}

			}

			else if(timer->Get() > 5 && timer->Get() < 10)
			{

			}
			else if(timer->Get() > 10 && timer->Get() < 15)
			{
				robotDrive->MecanumDrive_Cartesian(0,-.2,0,0);
			}

		}

		void TeleopInit()
		{
		}

		void TeleopPeriodic()
		{
			robotDrive->MecanumDrive_Cartesian(rightStick->GetX()*(rightStick->GetThrottle()+1.0f)/2.0f, rightStick->GetY()*(rightStick->GetThrottle()+1.0f)/2.0f, rightStick->GetTwist()*(rightStick->GetThrottle()+1.0f)/2.0f, 0.0f);
			liftMotor->Set(min(max(leftStick->GetY()*0.75f + static_cast<float>(sin(GetClock()/1000.0f))*0.25f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
			liftState |= leftStick->GetRawButton(6);
			liftState &= !leftStick->GetRawButton(4);
			leftLiftServo->Set(90 * liftState);
			rightLiftServo->Set(90 * !liftState);
		}

		void TestPeriodic()
		{
			lw->Run();
		}
};

START_ROBOT_CLASS(Robot);
