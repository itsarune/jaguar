#include "encoder_pid.h"        //include relevant header
#include "tracking.h"

//sets the PID controller values for each instance
void pidSet(pid_info* pid,
  double p, double i, double d,
  double motor) {
    pid->p = p;                 //sets the proportional value
    pid->i = i;                 //sets the integral value
    pid->d = d;                 //sets the derivative value
    pid->motor = motor;         //stores which motor this PID info is relevant to
}
/*int getEncoderLeft() {
  return encoderGet(encoderLeft) - encoderLeftOffset;
}
int getEncoderRight() {
  return encoderGet(encoderRight) - encoderRightOffset;
}*/

/*
  This function uses a PID controller using information from the PID structure
  to move the robot based on sensor-readings (typically encocder-ticks)
*/

void Reset(pidData* data) {
  data->derivative = 0;
  data->error = 0;
  data->integral = 0;
  data->lastError = 0;
  data->sense = 0;
  data->speed = 0;
  data->errorLongTimeAgo = 0;
}

bool CalculatePID(pidData* data, pid_info* pid)
{
    //calculate the error from target to current readings
    data->error = data->target - data->sense;
    //printf("\nfinding the data error: %d, %f", data->error, data->sense);

    data->integral += data->error;                  //add the error to the integral
    //find the derivative by calculating the difference from the previous two
    //  errors
    data->derivative = data->error - data->lastError;

    //disable the integral until it comes into a usable range
    if(data->error == 0 || (abs(data->error) > (127/2))) { data->integral = 0; }

    //put the whole PID shenanigan together and calculate the speed
    data->speed = (pid->p*data->error) + (pid->i*data->integral) + (pid->d*data->derivative);

    //if the previous two errors were 0, then the robot has probably stopped,
    //  so exit the program
    if (((data->error == data->errorLongTimeAgo))) { data->speed = 0; return false; }

    //end of loop, current error becomes the last error for the next run
    if(millis()%300 < 2)
    {data->errorLongTimeAgo = data->error;}
    data->lastError = data->error;
    return true;
}

void encoderMotor(pid_info* pid, pid_info* pid_other, int targetLeft, int targetRight) {
  delay(20);
  encoderLeftOffset = encoderGet(encoderLeft); //Set offsets so that the encoders don't have to be reset
  encoderRightOffset = encoderGet(encoderRight);

  rightData.target = targetRight;
  leftData.target = targetLeft;
  printf("john%d\n", rightData.target);
  printf("otherjohn%d", leftData.target);


  timeout = 10*((abs(targetLeft) + abs(targetRight)) / 2)/2.54 + millis();
  //printf("returned timeout%d", timeout);

  Reset(&rightData);
  Reset(&leftData);

  //variable holding sensor information (encoder)
  //resets the integral value
  bool runRight = true;
  bool runLeft = true;//start the PID controller
  //initialize the error, derivative and resulting speed values

  while(runRight || runLeft) {
    //printf("PID data: %f, target: %d", pid->p, target);
    rightData.sense = encoderGet(encoderRight) - encoderRightOffset;
    leftData.sense = encoderGet(encoderLeft) - encoderLeftOffset; //get encoder readings
    //printf("\nsense%f, %f", *(&rightData.sense), *(&leftData.sense));

    if(runRight) {runRight = CalculatePID(&rightData, pid);}
    if(runLeft) {runLeft = CalculatePID(&leftData, pid_other);}
    if (millis()%25 <= 2) {
      printf("\nRight: %d,%d\n", rightData.error, rightData.speed);
      printf("Left: %d,%d \n", leftData.error, leftData.speed);
    }

    chassisSet(leftData.speed,rightData.speed);        //request the calculated motor speed
    //tracking();
    delay(2);
  }
}
//calculates the ratio in which the robot moves in proportion to the number of
//  ticks
