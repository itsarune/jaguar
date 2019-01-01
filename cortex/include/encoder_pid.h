#ifndef _ENCODER_PID_H_
#define _ENCODER_PID_H_

//include other important functions
#include "main.h"
#include "motorslew.h"
#include "encoder_pid.h"
#include "chassis.h"

#define turn_ratio 3.73

typedef struct pid {

  double p, i, d, motor;

} pid_info;

typedef struct pidData {
  float sense;
  int lastError, errorLongTimeAgo;
  int integral;
  int error, derivative, speed;
  int target;
  } pidData;
/*
  Stores the PID information
*/

//declare PID controllers
pid_info driveStraightRight;
pid_info driveStraightLeft;
pid_info driveTurnRight;
pid_info driveTurnLeft;
pidData rightData;
pidData leftData;

bool CalculatePID(pidData* data, pid_info* pid);

/*
  Sets the ratio for encoder-based turns

*/

void encoderMotor(pid_info* pid, pid_info* pid_other, int targetLeft, int targetRight);
/*
  Uses the quad encoder and PID controller to reach the PID target
*/

void pidSet(pid_info* pid,
  double p, double i, double d,
  double motor);
/*
  Sets PID information
*/
/*
  Turn the robot to specific angles
*/

void Reset(pidData* data);

int timeout;
int encoderLeftOffset;
int encoderRightOffset;

#endif
