/*
 * Calibration.h
 *
 *  Created on: Feb 14, 2015
 *      Author: robbiemarkwick
 */

#ifndef SRC_CALIBRATION_H_
#define SRC_CALIBRATION_H_

namespace Calibration
{

    const double CARRY_XPR = 1.85;

    const double MAX_ACCEL_X = 0.02;
    const double MAX_ACCEL_Y = 0.015;
    const double MAX_ACCEL_MAG = 0.02;
    const double MAX_ACCEL_R = 0.02;
    const double MAX_ACCEL_LIFT = 0.05;

    const double ROT_COMP_CONTROL_SENSOR_RATIO = -0.00425;
    const double ROT_COMP_TOLERANCE = 0.05; // 0.2;
}

#endif /* SRC_CALIBRATION_H_ */
