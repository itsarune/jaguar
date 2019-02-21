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
#include "tracking.h"
#include "autonomous.h"

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

float encoderConstant = 0.015;

void operatorControl() {
	/*while (1) {
		if (joystickGetDigital(1,7, JOY_LEFT)) {
			motorReq(rollerIntake, 0);
			break;
		}
		printf("ready?\n");
		//motorReq(rollerIntake, 100);
		delay(20);
	}*/
	encoderReset(encoderRight);
	encoderReset(encoderLeft);
	//encoderMotorAutonomous(autonBackLeft, autonBackRight, 1000, 1000);
	//
	//
	//taskCreate(shoot, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

	//this variable ensures that each movement was meant to occur rather
	//than a roaming joystick
	int joythresh = 10;
	//int turnJoy = 50;                           				//sets the power of the motor
	bool run = true;
	int intakeSpeed = 127;
	float rightSpeed = 0;
	float leftSpeed = 0;
	float timeOfLastShot = 0;
	bool armStraightened = false;
	bool reverseRequired = false;
	int reverseMultiplier = 1;
	float timeOfLastLift = 0;
    while (1) {
	    if (joystickGetDigital(1,7, JOY_LEFT)) {
		    motorReq(rollerIntake, 0);
		    break;
	    }
	    printf("ready?\n");
	    delay(20);
	}
    TaskHandle auton = taskCreate(autonomous, TASK_DEFAULT_STACK_SIZE, NULL,	TASK_PRIORITY_DEFAULT);
    delay(15000);
    taskDelete(auton)
	/*int prevEncoderLeft = 0;
	int prevEncoderRight = 0;
	float driftMultiplierRight = 1;
	float driftMultiplierLeft = 1;*/
	printf("start");
	while (1) {
		if (joystickGetDigital(1,7, JOY_LEFT)) {
			motorReq(rollerIntake, 0);
			break;
		}
		printf("ready?\n");
		delay(20);
	}
    TaskHandle auton = taskCreate(autonomous, TASK_DEFAULT_STACK_SIZE, NULL,	TASK_PRIORITY_DEFAULT);
    delay(15000);
    taskDelete(auton);
	//encoderTurn(90);
	/*encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 1000, 1000);
	encoderTurn(60);*/
	taskCreate(encoderMotor, TASK_DEFAULT_STACK_SIZE, NULL,	TASK_PRIORITY_DEFAULT);
	while(1) {
		run = true;
		if (joystickGetDigital(1, 7, JOY_LEFT))
		{
			run = true;
		}
		if (run) {
			//printf("printing");
			//count = encoderGet(encoderRight);
			//printf("\nthe encoder value%d, %d", count, encoderGet(encoderLeft));

			if (abs(joystickGetAnalog(1, 3)) > joythresh){
				leftSpeed = joystickGetAnalog(1,3);
			}
			if (abs(joystickGetAnalog(1, 2)) > joythresh){
				rightSpeed = joystickGetAnalog(1, 2);
			}
			if (abs(joystickGetAnalog(1, 2)) < joythresh){
				rightSpeed = 0;
			}
			if (abs(joystickGetAnalog(1, 3)) < joythresh){
				leftSpeed = 0;
			}
			if (joystickGetDigital(1, 5, JOY_UP) || joystickGetDigital(1, 6, JOY_UP))
			{
				encoderConstant = 0.03;
			}
			else { encoderConstant = 0.015;}

			if (joystickGetDigital(1, 8, JOY_LEFT)) {
				reverseMultiplier = -1;
			}
			if (joystickGetDigital(1, 8, JOY_RIGHT)) {
				reverseMultiplier = 1;
			}
			float leftMove= leftSpeed * encoderConstant * reverseMultiplier;
			float rightMove = rightSpeed * encoderConstant * reverseMultiplier;

			//chassisSet(leftSpeed, rightSpeed);
			if(abs(rightMove) > 0 || abs(leftMove) > 0) {
				if(reverseMultiplier == 1)
				{
					changeRightTarget(rightMove);
					changeLeftTarget(leftMove);
				}
				else
				{
					changeRightTarget(leftMove);
					changeLeftTarget(rightMove);
				}
			}
			delay(2);
			if (joystickGetDigital(1, 5, JOY_DOWN)) {
				motorReq(rollerIntake, -intakeSpeed);
			}
			else if(joystickGetDigital(1, 6, JOY_DOWN)) {
				motorReq(rollerIntake, intakeSpeed);
			}
			else {
				motorReq(rollerIntake, 0);
			}

			if (joystickGetDigital(1, 7, JOY_DOWN)) {
				motorReq(armMotor2, 40);
				timeOfLastLift = millis();
			}

			/*if (joystickGetDigital(1, 7, JOY_UP)) {
				motorReq(armMotor1, -127);
				motorReq(armMotor2, 40);
			}
			else if (joystickGetDigital(1, 7, JOY_DOWN)) {
				motorReq(armMotor1, 127);
				motorReq(armMotor2, -40);
			}
			else {motorReq(armMotor1, 0); motorReq(armMotor2, 0);}*/

			if (joystickGetDigital(1, 8, JOY_UP)) {
				motorReq(shooterMotor, -70);
				timeOfLastShot = millis();
			}
			if (joystickGetDigital(1, 8, JOY_DOWN)) {
				motorReq(shooterMotor, 128);
				timeOfLastShot = millis();
			}

			/*if (joystickGetDigital(1, 7, JOY_RIGHT))
			{
				run = false;
				rightSpeed = 0;
				leftSpeed = 0;
			}*/
			if (millis() > timeOfLastShot + 500)
			{
				motorReq(shooterMotor, 0);
			}
			if(millis() > timeOfLastLift + 500)
			{
				if(motorGet(armMotor2) > 10)
				{
					armStraightened = true;
					motorReq(armMotor1, -127);
					motorReq(armMotor2, 0);
					timeOfLastLift = millis();
				}
				else if(armStraightened)
				{
					reverseRequired = true;
					motorReq(armMotor1, 127);
					motorReq(armMotor2, -40);
					timeOfLastLift = millis();
					armStraightened = false;
				}
				else if(reverseRequired)
				{
					motorReq(armMotor1, 0);
					motorReq(armMotor2, 0);
					reverseRequired = false;
				}
			}
			/*if (millis() > timeOfLastLiftPress + 200)
			{
				if(motorGet(armMotor1) > 10)
				{
					motorReq(armMotor1, 0);
					motorReq(armMotor2, 0);
				}
				else if(reverseNeeded)
				{
					motorReq(armMotor1, 127);
					motorReq(armMotor2, -40);
					timeOfLastLiftPress = millis();
					reverseNeeded = false;
				}
			}*/
		}
		//tracking();
	}
}
