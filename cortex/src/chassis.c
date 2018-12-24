#include "chassis.h"                   //include relevant source file

void chassisSet(int left, int right) { //sets the speed of the drive system
  //motordrive systems
  motorReq(leftMotor1, left);
  motorReq(rightMotor1, -right);
  motorReq(leftMotor2, left);
  motorReq(leftMotor3, left);
  motorReq(rightMotor2, -right);
}
