/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
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
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void operatorControl() {
	encoderReset(encoderRight);
	taskCreate(motorslewing, TASK_DEFAULT_STACK_SIZE, NULL,	TASK_PRIORITY_DEFAULT);
	//_DRONE_CONTROL_

	//this variable ensures that each movement was meant to occur rather
	//than a roaming joystick
	int joythresh = 10;
	int turnJoy = 50;
	int power = 0;
	int turn = 0;                               				//sets the power of the motor
	bool run = false;
	printf("start");
	encoderMotor(&driveStraightRight, 500, &encoderRight);
	while(1) {
		printf("running, you bastard");
		if (joystickGetDigital(1, 7, JOY_LEFT))
		{
			run = true;
		}
		motorReq(6, 120);
		delay(1000);
		motorReq(6, 0);
		if (run) {
			//printf("printing");
			//count = encoderGet(encoderRight);
			//printf("\nthe encoder value%d, %d", count, encoderGet(encoderLeft));
			if (((abs(joystickGetAnalog(1, 1))) > turnJoy) || (abs(joystickGetAnalog(1, 2)) > turnJoy)) {
				turn = (joystickGetAnalog(1, 1)/4);
				power = (joystickGetAnalog(1,2)/4);
			} else if (abs(joystickGetAnalog(1, 3)) > joythresh) {
				power = joystickGetAnalog(1, 3);
				turn = joystickGetAnalog(1, 4);
			} else {
				power = 0;
				turn = 0;
			}
			chassisSet(power+turn, power-turn);
			delay(10);
			if (joystickGetDigital(1, 7, JOY_RIGHT))
			{
				run = false;
				power = 0;
				turn = 0;
			}
		}
		
	}

}
