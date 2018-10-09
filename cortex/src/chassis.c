#include "chassis.h"                   //include relevant source file

void chassisSet(int left, int right) { //sets the speed of the drive system
  //set drive train speeds
  motorSet(5, left);
  motorSet(3, left);
  motorSet(4, right);
  motorSet(2, right);
  printf("%d\n", left);
}
