#include "main.h"
#include "chassis.h"

void chassisSet(int left, int right) {
  motorReq(leftMotor, left);  //edit motor port for left drive
  motorReq(rightMotor, right); //edit motor port for right drive
}
