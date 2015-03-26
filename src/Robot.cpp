#include "WPILib.h"

using namespace std;

enum State
{
	//DriveToTote,
	//SetUpLift,
	LowerLift,
	GrabTote,
	LiftTote,
	TurnTote,
	DriveToZone,
	DropTote,
	TelePosition,
	StopRobot

};

class Robot : public IterativeRobot
{
	private:
		bool liftState;
		bool presetLifting0 = false;
		bool presetLifting1= false;
		bool presetLifting2 = false;
		
		double x, y, rotation, throttle;
		
		const unsigned int lvl0EncoderConstant = 0;
		const unsigned int lvl1EncoderConstant = lvl1EncoderConstant;
		const unsigned int lvl2EncoderConstant = lvl2EncoderConstant;

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


		Gyro*					gyro;

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
			robotDrive->SetCANJaguarSyncGroup(0);

			encoder			= new Encoder(0,1,false,Encoder::k4X);

			leftStick       = Joystick::GetStickForPort(0);
			rightStick      = Joystick::GetStickForPort(1);
			
			lowerLiftSensor = new DigitalInput(2);
			upperLiftSensor = new DigitalInput(3);
			
			leftLiftServo   = new Servo(0);
			rightLiftServo  = new Servo(1);

			presetLifting0 = false;
			presetLifting1 = false;
			presetLifting2 = false;


			encoder 		= new Encoder(0,1,false,Encoder::k4X);

			gyro			= new Gyro(1);

			timer 			= new Timer();


			CameraServer::GetInstance()->SetQuality(50);
			CameraServer::GetInstance()->StartAutomaticCapture("cam1");
			
		}

		void AutonomousInit()
		{

			timer->Reset();
			timer->Start();
			state = LowerLift;
		}

		void AutonomousPeriodic()
		{

//			if(state == DriveToTote){
//				robotDrive->MecanumDrive_Cartesian(0,0.6,0,0);
//				if(timer->Get()> 5){
//					robotDrive->MecanumDrive_Cartesian(0,0,0,0);
//					state = SetUpLift;
//				}
//			}
//			else if(state == SetUpLift){
//				if(){}
////				if(encoder->Get() < con)
////				{
////					liftMotor->Set(min(max((1 * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - 0.0f)/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
////				}
//				else
//				{
//					liftState = false;
//					leftLiftServo->Set(90 * liftState);
//					rightLiftServo->Set(90 * !liftState);
//					liftMotor->Set(0);
//					state == GrabTote;
//				}
//			}



			// the new stuf ends
			SmartDashboard::PutNumber("Auto state", state);
			if(state == LowerLift)
			{

				if(!(!lowerLiftSensor->Get()))
				{
					liftMotor->Set(min(max((-1 * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - 0.0f)/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else
				{
					liftMotor->Set(0);
					encoder->Reset();
					state = GrabTote;
				}
			}
			else if(state == GrabTote)
			{
				liftState = false;
				leftLiftServo->Set(90 * liftState);
				rightLiftServo->Set(90 * !liftState);
				state = LiftTote;
			}

			else if(state == LiftTote)
			{
				if(encoder->Get() < 97000)
				{
					liftMotor->Set(1);
				}
				else
				{
					liftMotor->Set(0);
					timer->Reset();
					gyro->Reset();
					state = DriveToZone;
				}
			}
			else if(state == DriveToZone)
			{
				if(timer->Get() < 2)
				{
					//robotDrive->MecanumDrive_Cartesian(0,0,-.3,0);
					//robotDrive->MecanumDrive_Cartesian(0,-.8,0,0);
					// at .8 speed it goes 73inches per sec
					if((abs(gyro->GetAngle()) <= 90))
					{
						robotDrive->MecanumDrive_Cartesian(0,0,-0.2,0);
						SmartDashboard::PutNumber("Gyro Angle",gyro->GetAngle());
					}
					else
					{
						robotDrive->MecanumDrive_Cartesian(0,0,0,0);
					}
				}

//				else if(timer->Get() <= 0 && timer->Get() <= .5){
//					robotDrive->MecanumDrive_Cartesian(0,-.8,0,0);
//				}
				else if(timer->Get() < (2+ 1.45))
				{
					robotDrive->MecanumDrive_Cartesian(0,-0.6,0,0);
				}
				else if(timer->Get() > (2+ 1.45))
				{
					robotDrive->MecanumDrive_Cartesian(0,0,0,0);
					state = DropTote;
				}

			}
			else if(state == DropTote)
			{
				if(!(!lowerLiftSensor->Get()))
				{
					liftMotor->Set(min(max((-1 * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - 0.0f)/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else
				{
					liftMotor->Set(0);
					liftState = true;
					leftLiftServo->Set(90 * liftState);
					rightLiftServo->Set(90 * !liftState);
					state = TelePosition;
				}

			}
			else if(state == TelePosition)
			{
				if(encoder->Get() < 97000)
				{
					liftMotor->Set(min(max((1 * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) * (1.0f - 0.0f)/2.0f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else
				{
					liftMotor->Set(0);
				}
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
			robotDrive->MecanumDrive_Cartesian(x * throttle, y * throttle, rotation * throttle, 0);
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
					liftMotor->Set(min(max((-1.0f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f), static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else
				{
					presetLifting0 =false;
				}
			}
			if(leftStick->GetRawButton(9) ||  presetLifting1)
			{
				presetLifting1 = true;
				if(encoder->Get() > lvl1EncoderConstant)
				{
					liftState= true;
					liftMotor->Set(min(max((-0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) , static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else if(encoder->Get() < lvl1EncoderConstant - 1000)
				{
					liftMotor->Set(min(max((0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f) , static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));

				}
				else
				{
					liftState = false;
					presetLifting1 =false;
				}
			}

			if( leftStick->GetRawButton(10)|| presetLifting2)
			{
				presetLifting2 = true;
				if(encoder->Get() > lvl2EncoderConstant)
				{
					liftState = true;
					liftMotor->Set(min(max((-0.8f * 0.95f + static_cast<float>(sin(GetClock() * 500.0f)) * 0.05f), static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
				}
				else if(encoder->Get() < lvl2EncoderConstant- 1000) //263000
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
			liftState |= leftStick->GetRawButton(6);
			liftState &= !leftStick->GetRawButton(4);
			leftLiftServo->Set(90 * liftState);
			rightLiftServo->Set(90 * !liftState);
		}
			// the new stuff ends


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

			SmartDashboard::PutNumber("Gyro Angle",gyro->GetAngle());
			SmartDashboard::PutBoolean("Lift Flap", liftState);
			SmartDashboard::PutBoolean("LowerLiftSensor is activated",!lowerLiftSensor->Get());

			SmartDashboard::PutBoolean("Lift Flap", liftState);
			SmartDashboard::PutBoolean("LowerLiftSensor is activated",!lowerLiftSensor->Get());
		}

};

START_ROBOT_CLASS(Robot);
