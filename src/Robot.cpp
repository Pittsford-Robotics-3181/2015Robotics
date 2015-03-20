#include "WPILib.h"

using namespace std;

class Robot : public IterativeRobot
{
	private:
		bool liftState;
		bool presetLifting1 = false;
		bool presetLifting2 = false;
		double x, y, rotation, throttle;
		
		LiveWindow*             lw;
		
		PowerDistributionPanel* pdp;

		CANTalon*               frontLeftMotor;
		CANTalon*               frontRightMotor;
		CANTalon*               rearRightMotor;
		CANTalon*               rearLeftMotor;
		CANTalon*               liftMotor;

		Encoder*				encoder;

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
			robotDrive->SetSafetyEnabled(false);
			robotDrive->SetInvertedMotor(RobotDrive::kFrontLeftMotor,  0);
			robotDrive->SetInvertedMotor(RobotDrive::kFrontRightMotor, 1);
			robotDrive->SetInvertedMotor(RobotDrive::kRearRightMotor,  1);
			robotDrive->SetInvertedMotor(RobotDrive::kRearLeftMotor,   0);

			encoder			= new Encoder(0,1,false,Encoder::k4X);

			leftStick       = Joystick::GetStickForPort(0);
			rightStick      = Joystick::GetStickForPort(1);
			
			lowerLiftSensor = new DigitalInput(2);
			upperLiftSensor = new DigitalInput(3);
			
			leftLiftServo   = new Servo(0);
			rightLiftServo  = new Servo(1);

			presetLifting1 = false;
			presetLifting2 = false;


			CameraServer::GetInstance()->SetQuality(50);
			CameraServer::GetInstance()->StartAutomaticCapture("cam1");
			
		}

		void AutonomousInit()
		{
		}

		void AutonomousPeriodic()
		{
		}

		void TeleopInit()
		{
			encoder->Reset();
			presetLifting1= false;
			presetLifting2 = false;
		}

		void TeleopPeriodic()
		{

			printDiagnostics();

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
//			if(leftStick->GetRawButton(7) && encoder->GetDistance() > 0 ){
//				liftMotor->Set(min(max((-1 * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - leftStick->GetThrottle())/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
//			}
			if(!lowerLiftSensor->Get())
			{
				encoder->Reset();
			}

			if(leftStick->GetRawButton(9) ||  presetLifting1)
			{
				presetLifting1 = true;
				if(encoder->Get() > 98000)
				{
					liftMotor->Set(min(max((-0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - leftStick->GetThrottle())/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else if(encoder->Get() < 97000)
				{
					liftMotor->Set(min(max((0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - leftStick->GetThrottle())/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));

				}
				else
				{
					presetLifting1 =false;
				}
			}

			if( leftStick->GetRawButton(10)|| presetLifting2)
			{
				presetLifting2 = true;
				if(encoder->Get() > 264000)
				{
					liftMotor->Set(min(max((-0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - leftStick->GetThrottle())/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else if(encoder->Get() < 263000)
				{
					liftMotor->Set(min(max((0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - leftStick->GetThrottle())/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));

				}
				else
				{
					presetLifting2 =false;
				}
			}


			if(!presetLifting1 && !presetLifting2)
			{
				liftMotor->Set(min(max((leftStick->GetY() * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - leftStick->GetThrottle())/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
			}
			liftState |= leftStick->GetRawButton(6);
			liftState &= !leftStick->GetRawButton(4);
			leftLiftServo->Set(90 * liftState);
			rightLiftServo->Set(90 * !liftState);
		}

		void TestPeriodic()
		{
			lw->Run();
		}

		void printDiagnostics()
		{
			SmartDashboard::PutNumber("Encoder Count", encoder->Get());
			SmartDashboard::PutNumber("Encoder Raw Count", encoder->GetRaw());
			SmartDashboard::PutNumber("Encoder Period", encoder->GetPeriod());
			SmartDashboard::PutNumber("Encoder Rate", encoder->GetRate());
			SmartDashboard::PutBoolean("Encoder Direction", encoder->GetDirection());
			SmartDashboard::PutBoolean("Encoder Stopped", encoder->GetStopped());
			SmartDashboard::PutBoolean("Lift Flap", liftState);
			SmartDashboard::PutBoolean("LowerLiftSensor is activated",!lowerLiftSensor->Get());
		}

};

START_ROBOT_CLASS(Robot);
