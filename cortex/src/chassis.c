#include "chassis.h"                   //include relevant source file

void chassisSet(int left, int right) { //sets the speed of the drive system
  //motordrive systems
  motorSet(leftMotor1, left);
  motorSet(rightMotor1, -right);
  motorSet(leftMotor2, left);
  motorSet(rightMotor2, -right);
}
