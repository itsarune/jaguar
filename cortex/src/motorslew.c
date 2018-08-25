#include "motorslew.h"
#include "main.h"

#define MOTOR_SLEW_RATE  10
#define MOTOR_TASK_DELAY 15

int motor_req[11];
int motor_speed[11];
bool motor_slew[11];

void motorslewing() {
  for(int i = 1; i <= 10; i++) {
    motor_req[i] = 0;
    motor_slew[i] = false;
    motor_speed[i] = 0;
  }
  while(true){
    for(int i = 1; i <= 10; i++){
      if(motor_slew[i]) {
        if(motor_req[i] > motor_speed[i]) {        //is the requested speed greater than the current speed?
          motor_speed[i] += MOTOR_SLEW_RATE;
          if(motor_speed[i] > motor_req[i]) {      //has the slew rate over-corrected
            motor_speed[i] = motor_req[i];
            motor_slew[i] = false;
          }
        } else if(motor_req[i] < motor_speed[i]) {
          motor_speed[i] -= MOTOR_SLEW_RATE;
          if(motor_speed[i] < motor_req[i]) {       //has it over-corrected
            motor_speed[i] = motor_req[i];
            motor_slew[i] = false;
          }
        } else {
          motor_slew[i] = false;
        }
      }
      motorSet(i, motor_speed[i]);                 //finally set the motorspeed
    }
    delay(MOTOR_TASK_DELAY);
  }
}

void motorReq(int channel, int speed) {
    motor_req[channel] = speed;
    motor_slew[channel] = true;
}
