#include "main.h"
#include "chassis.h"

void chassisSet(int left, int right) {
  motorReq(2, left);  //edit motor port for left drive
  motorReq(3, right); //edit motor port for right drive
}
