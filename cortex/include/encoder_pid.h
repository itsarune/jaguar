#ifndef _ENCODER_PID_H_
#define _ENCODER_PID_H_

typedef struct pid {

  double p, i, d, motor;

} pid_info;
/*
  Stores the PID information
*/

int ratio;
/*
  Sets the ratio for encoder-based turns
*/

void encoderMotor();
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

void encoderTurn(bool turn, float angle);
/*
  Turn the robot to specific angles
*/

#endif
