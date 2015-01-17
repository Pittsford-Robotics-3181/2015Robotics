#include "WPILib.h"
#include "ControlScheme.h"
#include "DriveSystem.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw;
	DriveSystem *drive;
	ControlScheme *controls;
	Joystick * joystick;
	RobotDrive * robotDrive;



	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
//		//Drive System
//		SpeedController* fl = new Talon(0);
//		SpeedController* fr = new Talon(1);
//		SpeedController* bl = new Talon(2);
//		SpeedController* br = new Talon(3);
//		Gyro* driveGyro = new Gyro(1);
//		drive = new DriveSystem(fl,fr,bl,br,driveGyro);
//		//Control Scheme
//		Joystick* driveStick = new Joystick(0);
//		controls = new ControlScheme(driveStick);


        lw = LiveWindow::GetInstance();
        joystick = Joystick::GetStickForPort(0);
        robotDrive = new RobotDrive(0,3,1,2);
        //Camera
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

	}

	void TeleopPeriodic()
	{
//		double x=0,y=0,r=0;
//		controls->getDriveControls(x,y,r);
//		ControlReferenceFrame referenceFrame = controls->getDriveReferenceFrame();
//		drive->driveRobot(x,y,r,referenceFrame);

        robotDrive->MecanumDrive_Cartesian(joystick->GetX(),joystick->GetY(), joystick->GetZ());

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
