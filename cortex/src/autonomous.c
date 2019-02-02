#include "autonomous.h"

void myAuton(int route, int position) {
  int turnMultiplierAuto;
  //red = 0, blue = 1; position
  //route 1 is away from flags, route 2 is near flags
  turnMultiplierAuto = (position == 0) ? 1 : -1;
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
    encoderTurn(-90*turnMultiplierAuto);
    flagShoot();
    motorReq(1, -127);
    encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 400, 400);
    motorReq(1, 0);
    encoderTurn(90*turnMultiplierAuto);
    motorReq(1, 127);
    encoderMotorAutonomous(autonBackLeft, autonBackRight, -700, -700);
    motorReq(1, 0);
    encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 1500, 1500);
    encoderTurn(90*turnMultiplierAuto);
    encoderMotorAutonomous(autonStraightLeft, autonStraightRight, 2000, 2000);
    chassisSet(127, 127);
    delay(2000);
    chassisSet(0, 0);
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
