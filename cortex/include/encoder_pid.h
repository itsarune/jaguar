#ifndef _ENCODER_PID_H_
#define _ENCODER_PID_H_

//include other important functions
#include "main.h"
#include "motorslew.h"
#include "encoder_pid.h"
#include "chassis.h"

typedef struct pid {

  double p, i, d, motor;

} pid_info;

typedef struct pidData {
  float sense;
  int lastError;
  int integral;
  int error, derivative, speed;
  int turnMultiplier;
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

bool CalculatePID(pidData* data, int target, pid_info* pid);

int ratio;
/*
  Sets the ratio for encoder-based turns

*/

void encoderMotor(pid_info* pid, pid_info* pid_other, int target, bool forwardLeft, bool forwardRight);
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
