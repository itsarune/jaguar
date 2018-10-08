#ifndef _CHASSIS_H_
#define _CHASSIS_H_

//include relevant source files
#include "chassis.h"
#include "main.h"
#include "motorslew.h"

//define motors
#define leftMotor1  1
#define leftMotor2  3
#define rightMotor1 2
#define rightMotor2 4

//set speed of drive system
void chassisSet(int left, int right);

#endif
