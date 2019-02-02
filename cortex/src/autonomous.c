#include "autonomous.h"

void myAuton(int route) {
  if (route == 1) {
    encoderReset(encoderRight);
    encoderReset(encoderLeft);
    taskCreate(motorslewing, TASK_DEFAULT_STACK_SIZE, NULL,	TASK_PRIORITY_HIGHEST);
    int autonEndTime = millis() + 15000;
    printf("Starting autonomous code at %d", (int)millis());
    encoderMotorAutonomous(autonBackLeft, autonBackRight, -1200, -1200);
    motorReq(1, 127);
    encoderMotorAutonomous(autonBackLeft, autonBackRight, -250, -250);

    motorReq(1,0);
    /*encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 250, 250);
    encoderTurn(-90);
    encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 250, 250);*/
    if ((int)millis() >= autonEndTime) {
      printf("Autonomous done!");
      return;
    }
  } else if (route == 2) {
    encoderReset(encoderRight);
    encoderReset(encoderLeft);
    taskCreate(motorslewing, TASK_DEFAULT_STACK_SIZE, NULL,	TASK_PRIORITY_HIGHEST);
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



    if ((int)millis() >= autonEndTime) {
      printf("Autonomous done!");
      return;
    }
  }
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
