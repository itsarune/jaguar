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
/*
  Stores the PID information
*/

//declare PID controllers
pid_info driveStraightRight;
pid_info driveStraightLeft;
pid_info driveTurnRight;
pid_info driveTurnLeft;

int ratio;
/*
  Sets the ratio for encoder-based turns

*/

void changeRightTarget(int target);
void changeLeftTarget(int target);

void encoderMotor(void * parameter);
/*
  Uses the quad encoder and PID controller to reach the PID target
*/

void pidSet(pid_info* pid,
  double p, double i, double d,
  double motor);
/*
  Sets PID information
*/

void intRatio(int encoderTicks, int angle);
/*
  Initializes the ratio for encoder-based turns
*/

void encoderTurn(float angle, Encoder* sensor_reading,
    pid_info* pid, pid_info* motor2);
/*
  Turn the robot to specific angles
*/

void changeOffsets(int right, int left);
/*
  Change the encoder offsets in PID
*/

#endif
