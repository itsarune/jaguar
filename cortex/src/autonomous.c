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
  motorReq(1, 127);
  encoderMotorAutonomous(autonBackLeft, autonBackRight, -1500, -1500);
  motorReq(1,0);
}
