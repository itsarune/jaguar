/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "encoder_pid.h"
#include "chassis.h"

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO() {
}

/*
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An auton omous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */
void initialize() {
  //define encoders, remember to change ports and review reverses
  encoderRight = encoderInit(1, 2, true);
  encoderLeft = encoderInit(3, 4, true);
  encoderReset(encoderRight);
  encoderReset(encoderLeft);

  //initialize controllers that contrain information for driving straight with right motor
  pidSet(&driveStraightRight, 0.25, 0, 0.2, rightMotor1);
  pidSet(&driveTurnRight, 0, 0, 0, rightMotor1);
  pidSet(&driveTurnLeft, 0, 0, 0, leftMotor1);
  pidSet(&driveStraightLeft, 0.3, 0.00015, 0.12, leftMotor1);
  pidSet(&autonStraightLeft, 0.65, 0.000001, 0.27, leftMotor1);
  pidSet(&autonStraightRight, 0.161, 0.016, 0.49, rightMotor1);
  pidSet(&autonBackRight, 0.125, 0.00000001, 0.17, rightMotor1);
  pidSet(&autonBackLeft, 0.18, 0.000000001, 0.13, leftMotor1);
}
