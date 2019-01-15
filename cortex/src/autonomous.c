#include "autonomous.h"

void myAuton() {
  encoderReset(encoderRight);
  encoderReset(encoderLeft);
  taskCreate(motorslewing, TASK_DEFAULT_STACK_SIZE, NULL,	TASK_PRIORITY_HIGHEST);
  int autonEndTime = millis() + 15000;
  printf("Starting autonomous code at %d", (int)millis());
  if ((int)millis() >= autonEndTime) {
    printf("Autonomous done!");
    return;
  }
  encoderMotorAutonomous(autonBackLeft, autonBackRight, -1100, -1100);
  motorReq(1, 127);
  encoderMotorAutonomous(autonBackLeft, autonBackRight, -400, -400);
  motorReq(1,0);
}

void flagShoot() {
  motorReq(shooterMotor, 128);
  delay(500);
  motorReq(shooterMotor, 0);
}
/* AUTON ROUTINES PSEUDOCODE
Front1:
intake first ball
fire first ball high
intake second ball
fire second ball low
move forwards and flip flag
turn towards center
move to cone
intake ball
run intake other dir to flip cap

Front2:


Back1:
Run forwards
Run intake to flip cone
turn towards back of field (l/R)
MOVE FORWARDS
flip cone w/balls
go to platform

Back2:





*/
