2015 Robot Code for Pittsford Panthers Robotics
===============================================
[![Build Status](https://travis-ci.org/Pittsford-Robotics-3181/2015Robotics.svg?branch=sirchangealot)](https://travis-ci.org/Pittsford-Robotics-3181/2015Robotics)

Here is a line-by-line explanation of my code, well at least the TeleopPeriodic() method, because that's where the stuff that people might find confusing is.
```
robotDrive->MecanumDrive_Cartesian(rightStick->GetX()*(rightStick->GetThrottle()+1.0f)/2.0f, rightStick->GetY()*(rightStick->GetThrottle()+1.0f)/2.0f, rightStick->GetTwist()*(rightStick->GetThrottle()+1.0f)/2.0f, 0.0f);
```
This is just a call to WPILib's Mecanum Drive method, with throttle added.
```
liftMotor->Set(min(max(leftStick->GetY()*0.75f + static_cast<float>(sin(GetClock()/1000.0f))*0.25f, static_cast<float>(-lowerLiftSensor->Get())), static_cast<float>(upperLiftSensor->Get())));
```
The sin of time is just some anti-wobble. The min/max works like this. First it checks if the joystick Y value (+ wobble) (range -1 to 1) against the lower lift sensor with max(). If the sensor is 1, then the stick works. If it's 0, then the stick can only be a positive value, moving it up. The min() call is just the opposite.
```
liftState |= leftStick->GetRawButton(6);
liftState &= !leftStick->GetRawButton(4);
leftLiftServo->Set(90 * liftState);
rightLiftServo->Set(90 * !liftState);
```
The |= expands to `liftState = liftState | leftStick->GetRawButto(6);`. &= does the same thing but with &. An | between liftState and button 6 makes it so when button 6 is 1, liftState will become 1, but if button 6 is 0, liftState will not change. The & does the opposite, changing liftState to 0 if button 4 is 1, but leaving it alone if button 4 is 0. Multiplying 90 by liftState on the left and !liftState on the right makes it so when liftState is 1 left will be 90 and right will be 0, and flip them when liftState is 0.
