#include "encoder_pid.h"
#include "main.h"

void pidSet(pid_info* pid,
  double p, double i, double d,
  double motor) {
    pid->p = p;
    pid->i = i;
    pid->d = d;
    pid->motor = motor;
  }

void encoderMotor(pid_info* pid, target, sensor_reading) {
  int lastError = 0;
  int integral = 0;
  bool run = true;
  int error, derivative, speed;

  while(run) {
    error = target - sensor_reading;

    integral += error;
    derivative = error - lastError;

    if (error == 0 || (abs(error) > (127/2)) { integral = 0; }

    speed = (pid->p*error) + (pid->i*integral) + (pid->d*derivative);

    motorReq(pid->motor, speed);

    lastError = error;

    delay(20);
  }
}

void intRatio(int encoderTicks, int angle) {
  ratio = encoderTicks / angle;
}

void encoderTurn(bool turn, float angle);
