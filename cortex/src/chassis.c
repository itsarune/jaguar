//include relevant source files
#include "chassis.h"
#include "main.h"

void chassisSet(int left, int right) { //sets the speed of the drive system
  motorReq(leftMotor, left);           //edit motor port for left drive
  motorReq(rightMotor, right);         //edit motor port for right drive
}
