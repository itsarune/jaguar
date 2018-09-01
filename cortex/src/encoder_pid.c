#include "main.h"               //include other important functions

//sets the PID controller values for each instance
void pidSet(pid_info* pid,
  double p, double i, double d,
  double motor) {
    pid->p = p;
    pid->i = i;
    pid->d = d;
    pid->motor = motor;
  }

/*
  This function uses a PID controller using information from the PID structure
  to move the robot based on sensor-readings (typically encocder-ticks)
*/
void encoderMotor(pid_info* pid, int target, int sensor_reading) {
  int lastError = 0;
  int integral = 0;
  bool run = true;
  int error, derivative, speed;

  while(run) {
    error = target - sensor_reading;

    integral += error;
    derivative = error - lastError;

    if(error == 0 || (abs(error) > (127/2))) { integral = 0; }

    speed = (pid->p*error) + (pid->i*integral) + (pid->d*derivative);

    motorReq(pid->motor, speed);

    if (error == 0 && lastError == 0) { run = false; }

    lastError = error;

    delay(20);
  }
}

void intRatio(int encoderTicks, int angle) {
  ratio = encoderTicks / angle;
}

void encoderTurn(bool turn, float angle, int sensor_reading,
    pid_info* pid, pid_info* motor2) {
  encoderMotor(pid, (angle*ratio), sensor_reading);
  angle *= -1;
  encoderMotor(motor2, (angle*ratio), sensor_reading);
}
