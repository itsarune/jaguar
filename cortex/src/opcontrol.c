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
	/*while(1){
	motorSet(5, 100);
	motorSet(2, 100);
	motorSet(3,100);
	motorSet(4,100);
}*/
	//taskCreate(motorslewing, TASK_DEFAULT_STACK_SIZE, NULL,
		//TASK_PRIORITY_DEFAULT);

	//this variable ensures that each movement was meant to occur rather
	//than a roaming joystick
	int joythresh = 11;
	int power = 0;
	int turn = 0;                               				//sets the power of the motor
	while (1) {
		/*if(abs(joystickGetAnalog(1, 4))>joythresh) {   			  //tank, drastic turns get priority
			//get the value of the x-axis of the left joystick
			power = joystickGetAnalog(1, 4);
			//sets the speed, right motor opposide the left
			chassisSet(power, -1*power);
			//has the right joystick moved enough to move the robot?
		} */
		if((abs(joystickGetAnalog(1, 1))>joythresh) ||
				(abs(joystickGetAnalog(1, 2))>joythresh)) {
					power = joystickGetAnalog(1, 2); 							  //y-axis of channel 2
					turn = joystickGetAnalog(1, 1);								  //x-axis of channel 1
					chassisSet((power+turn), (power-turn));							//tell the robot to drive
				}
		
		delay(2);
	}
}
