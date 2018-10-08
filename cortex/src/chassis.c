#include "chassis.h"                   //include relevant source file

void chassisSet(int left, int right) { //sets the speed of the drive system
  //set drive train speeds
  motorReq(leftMotor1, left);
  motorReq(leftMotor2, left);
  motorReq(rightMotor1, right);
  motorReq(rightMotor2, right);
}
