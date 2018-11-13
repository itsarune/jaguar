#include "encoder_pid.h"        //include relevant header

//sets the PID controller values for each instance
void pidSet(pid_info* pid,
  double p, double i, double d,
  double motor) {
    pid->p = p;                 //sets the proportional value
    pid->i = i;                 //sets the integral value
    pid->d = d;                 //sets the derivative value
    pid->motor = motor;         //stores which motor this PID info is relevant to
  }

/*
  This function uses a PID controller using information from the PID structure
  to move the robot based on sensor-readings (typically encocder-ticks)
*/
void encoderMotor(pid_info* pid, int target, Encoder* sensor_reading) {
  encoderReset(*sensor_reading);         //resets encoder
  //variable holding sensor information (encoder)
  float sense;
  int lastError = 0;                    //resets the last error
  int integral = 0;                     //resets the integral value
  bool run = true;                      //start the PID controller
  //initialize the error, derivative and resulting speed values
  int error, derivative, speed;

  while(run) {

    sense = encoderGet(*sensor_reading); //get encoder readings
    printf("\nsense%f.1", sense);

    //calculate the error from target to current readings
    error = target - sense;

    integral += error;                  //add the error to the integral
    //find the derivative by calculating the difference from the previous two
    //  errors
    derivative = error - lastError;

    //disable the integral until it comes into a usable range
    if(error == 0 || (abs(error) > (127/2))) { integral = 0; }

    //put the whole PID shenanigan together and calculate the speed
    speed = (pid->p*error) + (pid->i*integral) + (pid->d*derivative);

    chassisSet(speed, speed);        //request the calculated motor speed

    //if the previous two errors were 0, then the robot has probably stopped,
    //  so exit the program
    if (error == 0 && lastError == 0) { run = false; }

    //end of loop, current error becomes the last error for the next run
    lastError = error;
    lcdClear(uart1);
    lcdPrint(uart1, 1, "error is %d", error);
    delay(2);
  }
}

//calculates the ratio in which the robot moves in proportion to the number of
//  ticks
void intRatio(int encoderTicks, int angle) {
  ratio = encoderTicks / angle;          //simple ration calculation
}

//use encoders to try to make an accurate turn
void encoderTurn(float angle, Encoder* sensor_reading,
    pid_info* pid, pid_info* motor2) {
  //pass relevant information to motors
  //encoderMotor(pid, (angle*ratio));
  //negate the angle as the motor will turn the opposite way
  angle *= -1;
  //encoderMotor(motor2, (angle*ratio));
}
