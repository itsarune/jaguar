#include "autonomous.h"

void myAuton(int route, int position) {
  int turnMultiplier;
  turnMultiplier = (position == 0) ? 1 : -1;
  if (route == 1) {
    encoderReset(encoderRight);
    encoderReset(encoderLeft);
    taskCreate(motorslewing, TASK_DEFAULT_STACK_SIZE, NULL,	TASK_PRIORITY_HIGHEST);
    int autonEndTime = millis() + 15000;
    printf("Starting autonomous code at %d", (int)millis());
    encoderMotorAutonomous(autonBackLeft, autonBackRight, -1200, -1200);
    motorReq(1, -127);
    encoderMotorAutonomous(autonBackLeft, autonBackRight, -300, -300);
    delay(1000);
    motorReq(1,0);
    encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 500, 500);
    encoderTurn(-90*turnMultiplier);
    flagShoot();
    motorReq(1, -127);
    encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 400, 400);
    motorReq(1, 0);
    encoderTurn(90*turnMultiplier);
    motorReq(1, 127);
    encoderMotorAutonomous(autonBackLeft, autonBackRight, -700, -700);
    motorReq(1, 0);
    /*encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 250, 250);
    encoderTurn(-90);
    encoderMotorAuto nomous(autonStraightLeft, autonStraightRight, 250, 250);*/
    if ((int)millis() >= autonEndTime) {
      printf("Autonomous done!");
      return;
    }
  } else if (route == 2) {
      encoderTurn(-45*turnMultiplier);
      motorReq(1, 127);
      encoderMotorAutonomous(autonBackLeft, autonBackRight, -600, -600);
      motorReq(1, 0);
      encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 600, 600);
      encoderTurn(-45*turnMultiplier);
      encoderMotorAutonomous(autonBackLeft, autonBackRight, -1500, -1500);
      encoderMotorAutonomous(autonBackLeft, autonBackRight, 1500, 1500);
      return;
  }
}

void flagShoot() {
  motorReq(shooterMotor, 127);
  delay(2000);
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
