#include "WPILib.h"

using namespace std;

class Robot : public IterativeRobot
{
	private:
		bool liftState;
		bool binState;
		bool cancelPreset;
		bool presetLifting0 = false;
		bool presetLifting1 = false;
		bool presetLifting2 = false;
		double x, y, rotation, throttle;
		
		const unsigned int lvl0EncoderConstant = 0;
		const unsigned int lvl1EncoderConstant = 98000;
		const unsigned int lvl2EncoderConstant = 264000;

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
		
		DigitalInput*			autoStart1;
		DigitalInput*			autoStart2;
		DigitalInput*			autoStart3;
		DigitalInput*			autoStart4;

		Servo*                  leftLiftServo;
		Servo*                  rightLiftServo;
		
		Timer*					timer;

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

			autoStart1		= new DigitalInput(4);
			autoStart2		= new DigitalInput(5);
			autoStart3		= new DigitalInput(6);
			autoStart4		= new DigitalInput(7);

			presetLifting0 	= false;
			presetLifting1 	= false;
			presetLifting2 	= false;

			binState		= false;
			timer			= new Timer();

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
			if(!autoStart1->Get()) // Move backwords
			{
				if(timer->Get() < 2.5)
				{
					robotDrive->MecanumDrive_Cartesian(0,0.4,0,0);
				}
				else
				{
					robotDrive->MecanumDrive_Cartesian(0,0,0,0);
				}
			}
			else if(!autoStart2->Get()) //Move backwords with tote
			{
				if(timer->Get() < 0.5)
				{
					liftMotor->Set(min(max((0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) , static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else if(timer->Get() < (.5 +2.5))
				{
					liftMotor->Set(0);
					robotDrive->MecanumDrive_Cartesian(0,0.4,0,0);
				}
				else
				{
					robotDrive->MecanumDrive_Cartesian(0,0,0,0);
				}
			}
			else if(!autoStart3->Get()) // Pick up green bin and move back
			{
				if(timer->Get() < 1)
				{
					liftMotor->Set(min(max((0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) , static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else if(timer->Get() < (1 +2.5))
				{
					liftMotor->Set(0);
					//robotDrive->MecanumDrive_Cartesian(0,0.5,0,0);
					robotDrive->MecanumDrive_Cartesian(0,0.4,0,0);
					//robotDrive->MecanumDrive_Cartesian(0,((2.5- (timer->Get()/2))/2.5)*0.6,0,0);
				}
				else
				{
					liftMotor->Set(0);
					robotDrive->MecanumDrive_Cartesian(0,0,0,0);
				}
			}
			else if(!autoStart4->Get()) // Pick up green bin , move back, and make it over the step
			{
				if(timer->Get() < 1)
				{
					liftMotor->Set(min(max((0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) , static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else if(timer->Get() < (1 +2.5))
				{
					liftMotor->Set(0);
					//robotDrive->MecanumDrive_Cartesian(0,0.5,0,0);
					robotDrive->MecanumDrive_Cartesian(0,0.6,0,0);
					//robotDrive->MecanumDrive_Cartesian(0,((2.5- (timer->Get()/2))/2.5)*0.6,0,0);
				}
				else
				{
					liftMotor->Set(0);
					robotDrive->MecanumDrive_Cartesian(0,0,0,0);
				}

			}
			else //Not Moving
			{

			}

		}

		void TeleopInit()
		{
			encoder->Reset();
			presetLifting0 = false;
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

			if(leftStick->GetRawButton(11) ||  presetLifting0)
			{
				presetLifting0 = true;
				if(!(!lowerLiftSensor->Get()))
				{
					liftState = true;
					liftMotor->Set(min(max((-1.0f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f *fabs(leftStick->GetY()))) * 0.05f), static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else
				{
					presetLifting0 =false;
				}
			}
			if(leftStick->GetRawButton(9) ||  presetLifting1)
			{
				presetLifting1 = true;
				if(encoder->Get() > 98000)
				{
					liftState= true;
					liftMotor->Set(min(max((-0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) , static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else if(encoder->Get() < 97000)
				{
					liftMotor->Set(min(max((0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) , static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));

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
					liftState = true;
					liftMotor->Set(min(max((-0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f), static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else if(encoder->Get() < 263000) //263000
				{
					liftMotor->Set(min(max((0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) , static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));

				}
				else
				{
					presetLifting2 =false;
				}
			}
			presetLifting0 &= fabs(leftStick->GetY()) < 0.5;
			presetLifting1 &= fabs(leftStick->GetY()) < 0.5;
			presetLifting2 &= fabs(leftStick->GetY()) < 0.5;

			if(!presetLifting1 && !presetLifting2 && !presetLifting0)
			{
				liftMotor->Set(min(max((leftStick->GetY() * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - leftStick->GetThrottle())/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
			}
			if(leftStick->GetRawButton(3))
			{
				binState = true;
			}
			else if(leftStick->GetRawButton(4) || leftStick->GetRawButton(6) || leftStick->GetRawButton(9) || leftStick->GetRawButton(10) || leftStick->GetRawButton(11))
			{
				binState = false;
			}


			if(binState)
			{
					leftLiftServo->SetAngle(30);
					rightLiftServo->SetAngle(60);
			}
			else
			{
			liftState |= leftStick->GetRawButton(6);
			liftState &= !leftStick->GetRawButton(4);
			leftLiftServo->SetAngle(90 * liftState);
			rightLiftServo->SetAngle(90 * !liftState);
			}
		}

		void TestPeriodic()
		{
			lw->Run();
		}

		void printDiagnostics()
		{
			SmartDashboard::PutBoolean("autoStart 1", autoStart1->Get());
			SmartDashboard::PutBoolean("autoStart 2", autoStart2->Get());
			SmartDashboard::PutBoolean("autoStart 3", autoStart3->Get());
			SmartDashboard::PutBoolean("autoStart 4", autoStart4->Get());
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
