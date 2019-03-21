//include relevant source file
#include "motorslew.h"

//rate in which the motor steps
#define MOTOR_SLEW_RATE  10
//delay to request motor speed
#define MOTOR_TASK_DELAY 10

//requested motor speeds for all motors
int motor_req[11];
//current motor speed for all motors
int motor_speed[11];
//is this motor being slewed?
bool motor_slew[11];

bool motor_pid_run[11];
int motor_pid[11];
pidData motorPIDData[11];

typedef struct pidData {
  float sense;
  double p,i,d;
  int lastError, errorLongTimeAgo;
  int integral;
  int error, derivative, speed;
  int target, lastTarget, timePassed;
} pidData;

void Reset(pidData* data)
{
  data->derivative = 0;
  data->error = 0;
  data->integral = 0;
  data->lastError = 0;
  data->errorLongTimeAgo = 0;
  data->sense = 0;
  data->speed = 0;
  data->target = 0;
  data->lastTarget = 0;
  data->timePassed = 0;
}

int getEncoderLeft() {
  return encoderGet(encoderLeft) + encoderLeftOffset;
}
int getEncoderRight() {
  return encoderGet(encoderRight) + encoderRightOffset;
}

void beginPID(int motor, int target) {
  motor_pid_run[motor] = true;
  motor_pid[motor] = target;
}

void resetPID() {
  for (int i = 1; i < 11; i += 1) 
    Reset(motorPid[i]); 
}

void encoderMotor(pid_info leftPID, pid_info rightPID, int target) {
  delay(20);
  encoderLeftOffset = getEncoderLeft();
  encoderRightOffset = getEncoderRight();

  //function();

  //timeout = 10*((abs(targetLeft) + abs(targetRight)) / 2)/2.54 + millis();
  //printf("returned timeout%d", timeout);

  Reset(&rightDataAuton);
  Reset(&leftDataAuton);
  rightDataAuton.target = targetRight;
  leftDataAuton.target = targetLeft;

  //variable holding sensor information (encoder)
  //resets the integral value
  bool runRight = true;
  bool runLeft = true;//start the PID controller
  //initialize the error, derivative and resulting speed values

  while(runRight || runLeft) {
    //printf("PID data: %f, target: %d", pid->p, target);
    rightDataAuton.sense = getEncoderRight();
    leftDataAuton.sense = getEncoderLeft(); //get encoder readings
    //printf("\nsense%f, %f", *(&rightData.sense), *(&leftData.sense));

    if(runRight) {runRight = CalculatePIDAuto(&rightDataAuton, rightPID);}
    if(runLeft) {runLeft = CalculatePIDAuto(&leftDataAuton, leftPID);}
    if (millis()%25 <= 2) {
      printf("\nRight: %d,%d\n", rightDataAuton.error, rightDataAuton.speed);
      printf("Left: %d,%d, %f \n", leftDataAuton.error, leftDataAuton.speed, leftPID.p);
    }

    chassisSet(leftDataAuton.speed,rightDataAuton.speed);        //request the calculated motor speed
    tracking();
    delay(2);
  }
}

int pidData CalculatePID(pidData* data)
{
    /*if (millis()%40 <= 2)
      printf("PID prop: %f\n", pid.p);*/
    //calculate the error from target to current readings
    data.error = data.target - data.sense;
    data.integral += data.error;                  //add the error to the integral
    //find the derivative by calculating the difference from the previous two
    //  errors
    data.derivative = data.error - data.lastError;

    //disable the integral until it comes into a usable range
    if(data.error == 0 || (abs(data.error) > (127/2))) { data.integral = 0; }

    //put the whole PID shenanigan together and calculate the speed
    data.speed = (pid.p*data.error) + (pid.i*data.integral) + (pid.d*data.derivative);

    //if the previous two errors were 0, then the robot has probably stopped,
    //  so exit the program
    if ((abs(data.error) <= 0 && abs(data.lastError) <= 0) || (data.target == data.lastTarget && data.error == data.lastError)) {
      data.speed = 0;
      data.target = data.sense;
      //printf("exit speed\n");
    }

    //end of loop, current error becomes the last error for the next run
    data.lastError = data.error;
    if(millis()%400 <= 3)
    { data.lastTarget = data.target;
    //  printf("Right: E %d, N %f, T %d, S %d\n", rightData.error, rightData.sense, rightData.target, rightData.speed);
    //printf("Left: E %d, N %f, T %d, S %d\n", leftData.error, leftData.sense, leftData.target, leftData.speed);
    }

    return int(data.speed);;
}

void motorslewing(void * parameter) {              //motor slew function
  for(int i = 1; i <= 10; i++) {                   //resets all variables
    //resets all requested motor speeds
    motor_req[i] = 0;
    //resets all motor slewing states
    motor_slew[i] = false;
    //rests all motors current speed
    motor_speed[i] = 0;
    motor_pid_run[i] = false;
    motor_pid[i] = 0;
  }
  //repeating function through the program
  while(true){
    //check through every motor in the program
    for(int i = 1; i <= 10; i++){
      if(motor_pid_run[i]) {
        motor_req[i] = calculatePID(&motorPIDData[i]);
        //motor_req[i
      }
      if(motor_slew[i]) {                          //does it require slewing
        //is the requested speed greater than the current speed?
        if(motor_req[i] > motor_speed[i]) {
          motor_speed[i] += MOTOR_SLEW_RATE;       //add the slew value
          //has the slew rate over-corrected
          if(motor_speed[i] > motor_req[i]) {
            //then set the requested speed
            motor_speed[i] = motor_req[i];
            //the motor need not be slewed anymore, so request can be turned
            //  off
            motor_slew[i] = false;
          }
        } else if(motor_req[i] < motor_speed[i]) { //is a slower speed required
          motor_speed[i] -= MOTOR_SLEW_RATE;       //lower the speed reasonably
          if(motor_speed[i] < motor_req[i]) {      //has it over-corrected
            //set the actual requested speed
            motor_speed[i] = motor_req[i];
            //the motor need not be slewed anymore, so request can be turned
            //  off
            motor_slew[i] = false;
          }
        } else {
          //set the required speed as last slewing already set the required speed
          motor_speed[i] = motor_req[i];
          //required speed set, no longer needs to be slewed
          motor_slew[i] = false;
        }
      }
      motorSet(i, motor_speed[i]);                 //finally set the motorspeed
    }
    delay(MOTOR_TASK_DELAY);
  }
}

void motorReq(int channel, int speed) {            //speed request
    //set the requested speed for the requested motor
    motor_req[channel] = speed;
    motor_slew[channel] = true;                    //motor requires slewing
}
