/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "chassis.h"
#include "motorslew.h"

/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */
void autonomous() {
  taskCreate(motorslewing, TASK_DEFAULT_STACK_SIZE, NULL,	TASK_PRIORITY_HIGHEST);
  myAuton(1,1);

}


void myAuton(int route, int position) {
  int turnMultiplierAuto = (position == 0) ? 1 : -1;
  if (route == 1) {
    while (encoderGet(encoderLeft) < 300) {
        chassisSet(127 * 0.55, 127);
    }
    chassisSet(0,0);
    flagShoot();
    encoderReset(encoderLeft);
    while (encoderGet(encoderLeft) < 800) {
        chassisSet(127 * 0.55, 127);
    }
    chassisSet(0,0);
    encoderReset(encoderLeft);
    while (abs(encoderGet(encoderLeft)) < 400) {
          chassisSet(-127*0.5, -127);
     }
    chassisSet(0,0);
    encoderReset(encoderLeft);
    while (abs(encoderGet(encoderLeft)) < 360) {
      chassisSet(50, -50);
    }
    chassisSet(0,0);
    /*encoderTurn(-90*turnMultiplierAuto);
    while (abs(encoderGet(encoderLeft)) < 1600) {
          chassisSet(-127*0.55, -127);
     }
    chassisSet(0,0);*/
  //red = 0, blue = 1; position
  //route 1 is away from flags, route 2 is near flags
  } else if (route == 2) {
      turnMultiplierAuto = (position == 0) ? 1 : -1;
      encoderReset(encoderLeft);
      while (encoderGet(encoderLeft) < 600) {
         chassisSet(127 * 0.55, 127);
      }
      chassisSet(0,0);
      encoderReset(encoderLeft);
      encoderTurn(90*turnMultiplierAuto);
      return;
  } else if (route == 3) {
      encoderReset(encoderLeft);
      while (encoderGet(encoderLeft) < 800) {
         chassisSet(127 * 0.55, 127);
      }
      chassisSet(0,0);
      encoderReset(encoderLeft);
      motorReq(rollerIntake, -127);
      motorReq(topIntake, -127);
      while (encoderGet(encoderLeft) < 250) {
         chassisSet(127 * 0.55, 127);
      }
      chassisSet(0,0);
      delay(2000);
      motorReq(rollerIntake, 0);
      motorReq(topIntake, 0);
      encoderReset(encoderLeft);
  }
}

void flagShoot() {
  motorReq(shooterMotor, 127);
  delay(1500);
  motorReq(shooterMotor, 0);
}
