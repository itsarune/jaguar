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
#include "encoder_pid.h"
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
  myAuton(1,0);
}


void myAuton(int route, int position) {
  int turnMultiplierAuto;
  //red = 0, blue = 1; position
  //route 1 is away from flags, route 2 is near flags
  turnMultiplierAuto = (position == 0) ? 1 : -1;
  if (route == 1) {
    encoderReset(encoderRight);
    encoderReset(encoderLeft);
    int autonEndTime = millis() + 15000;
    printf("Starting autonomous code at %d", (int)millis());
    encoderMotorAutonomous(autonBackLeft, autonBackRight, -1200, -1200);
    motorReq(1, -127);
    encoderMotorAutonomous(autonBackLeft, autonBackRight, -300, -300);
    delay(1000);
    motorReq(1,0);
    encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 500, 500);
    encoderTurn(-90*turnMultiplierAuto);
    flagShoot();
    motorReq(1, -127);
    encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 400, 400);
    motorReq(1, 0);
    encoderTurn(90*turnMultiplierAuto);
    motorReq(1, 127);
    encoderMotorAutonomous(autonBackLeft, autonBackRight, -700, -700);
    motorReq(1, 0);
    /*encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 1500, 1500);
    encoderTurn(90*turnMultiplierAuto);
    encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 2000, 2000);
    chassisSet(127, 127);
    delay(2000);
    chassisSet(0, 0);*/
    /*encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 250, 250);
    encoderTurn(-90);
    encoderMotorAuto nomous(autonStraightLeft, autonStraightRight, 250, 250);*/
    if ((int)millis() >= autonEndTime) {
      printf("Autonomous done!");
      return;
    }
  } else if (route == 2) {
      int autonEndTime = millis() + 15000;
      printf("Starting autonomous code at %d", (int)millis());
      encoderMotorAutonomous(autonBackLeft, autonBackRight, -400, -400); //move forward to shooting position
      flagShoot(); //shoot flag
      encoderMotorAutonomous(autonBackLeft, autonBackRight, -1500, -1500); //hit low flag
      encoderMotorAutonomous(autonBackLeft, autonBackRight, 1900, 1900); //return to spawn
      encoderTurn(90*turnMultiplierAuto); //turn towards platform
      encoderMotorAutonomous(autonBackLeft, autonBackRight, -1200, -1200); //move towards tilted cone
      motorReq(1, 127); //start intake
      encoderMotorAutonomous(autonBackLeft, autonBackRight, -250, -250); //yeet the ball
      motorReq(1,0); //stop intake
      encoderMotorAutonomous(autonBackLeft, autonBackRight, 450, 450);
      encoderTurn(90*turnMultiplierAuto); //turn towards platform
      encoderMotorAutonomous(autonBackLeft, autonBackRight, -1600, -1600); //hop onto platform
      return;
  }
}

void flagShoot() {
  motorReq(shooterMotor, 127);
  delay(1500);
  motorReq(shooterMotor, 0);
}