#include "chassis.h"                   //include relevant source file

void chassisSet(int left, int right) { //sets the speed of the drive system
  //motordrive systems
  motorReq(leftMotor1, left);
  motorReq(leftMotor2, left);
  motorReq(leftMotor3, left);
  if(abs(right) < 90) //VALUE NEEDS TESTING WITH OPCONTROL PID
  {
    motorReq(rightMotor1, -right);
    motorReq(rightMotor2, -right);
    motorReq(rightMotor3, -right);
  }
  else
  {
    float multiplier = right > 0 ? 1 : -1;
    motorReq(rightMotor1, 90 * multiplier);
    motorReq(rightMotor2, 90 * multiplier);
    motorReq(rightMotor3, 90 * multiplier);
  }
}
