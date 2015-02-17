/*
 * Hardware.h
 *
 *Created on: Feb 14, 2015
 *Author: robbiemarkwick
 */

#ifndef SRC_HARDWARE_H_
#define SRC_HARDWARE_H_

namespace Hardware
{
// MOTORS
const uint32_t frontLeftDriveMotor = 1;
const uint32_t frontRightDriveMotor = 2;
const uint32_t backLeftDriveMotor = 4;
const uint32_t backRightDriveMotor = 3;

const uint32_t liftMotor = 5;
const uint32_t leftServo = 0;
const uint32_t rightServo = 1;

const uint8_t powerDistributionChannelLiftMotor = 3;

// SENSORS
const uint32_t driveRotationGyro = 0;

const uint32_t liftEncoderPort1 = 0;
const uint32_t liftEncoderPort2 = 1;

const uint32_t liftProxUpper = 3;
const uint32_t liftProxLower = 2;

const uint32_t sonarPingL = 6;
const uint32_t sonarEchoL = 7;

const uint32_t sonarPingR = 8;
const uint32_t sonarEchoR = 9;
}

#endif /* SRC_HARDWARE_H_ */
