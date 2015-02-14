/*
 * Hardware.h
 *
 *  Created on: Feb 14, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_HARDWARE_H_
#define SRC_HARDWARE_H_

namespace Hardware {
//MOTORS
const uint32_t frontLeftDriveMotor = 1;
const uint32_t frontRightDriveMotor = 2;
const uint32_t backLeftDriveMotor = 4;
const uint32_t backRightDriveMotor = 3;

const uint32_t liftMotor = 5;
const uint32_t leftServo = 0;
const uint32_t rightServo = 1;

//SENSORS
const uint32_t driveRotationGyro = 0;

const uint32_t liftEncoderPort1 = 0;
const uint32_t liftEncoderPort2 = 1;

const uint32_t liftLimitUpper = 3;
const uint32_t liftLimitLower = 2;

const uint32_t flapLimitUpper = 5;
const uint32_t flapLimitLower = 4;

const uint32_t sonarPing = 7;
const uint32_t sonarEcho = 8;
}

#endif /* SRC_HARDWARE_H_ */
