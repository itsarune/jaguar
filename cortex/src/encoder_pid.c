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

int encoderLeftOffset;
int encoderRightOffset;

int getEncoderLeft() {
  return encoderGet(encoderLeft) - encoderLeftOffset;
}
int getEncoderRight() {
  return encoderGet(encoderRight) - encoderRightOffset;
}

/*
  This function uses a PID controller using information from the PID structure
  to move the robot based on sensor-readings (typically encocder-ticks)
*/
struct pidData {
  float sense;
  int lastError;
  int integral;
  int error, derivative, speed;
  int turnMultiplier;
} pidData;

void Reset(struct pidData data)
{
  data.derivative = 0;
  data.error = 0;
  data.integral = 0;
  data.lastError = 0;
  data.sense = 0;
  data.speed = 0;
  data.turnMultiplier = 1;
}

 int timeout;

bool CalculatePID(struct pidData data, int target, pid_info* pid)
{
    //calculate the error from target to current readings
    data.error = target*data.turnMultiplier - data.sense;

    data.integral += data.error;                  //add the error to the integral
    //find the derivative by calculating the difference from the previous two
    //  errors
    data.derivative = data.error - data.lastError;

    //disable the integral until it comes into a usable range
    if(data.error == 0 || (abs(data.error) > (127/2))) { data.integral = 0; }

    //put the whole PID shenanigan together and calculate the speed
    data.speed = (pid->p*data.error) + (pid->i*data.integral) + (pid->d*data.derivative);

    //if the previous two errors were 0, then the robot has probably stopped,
    //  so exit the program
    if (((data.error == 0 && data.lastError == 0)) || (int)millis() >= timeout) { data.speed = 0; return false; }

    //end of loop, current error becomes the last error for the next run
    data.lastError = data.error;
    return true;
}

struct pidData leftData;
struct pidData rightData;

void encoderMotor(pid_info* pid, int target, bool forwardLeft, bool forwardRight) {
  encoderLeftOffset = encoderGet(encoderLeft); //Set offsets so that the encoders don't have to be reset
  encoderRightOffset = encoderGet(encoderRight);
  
  Reset(rightData);
  Reset(leftData);

  rightData.turnMultiplier = forwardRight == true ? 1 : -1;
  leftData.turnMultiplier = forwardLeft == true ? 1 : -1;
  
  //variable holding sensor information (encoder)
  //resets the integral value
  bool runRight = true;
  bool runLeft = true;//start the PID controller
  //initialize the error, derivative and resulting speed values

  while(runLeft || runLeft) {   
    timeout = millis() + 10*target/2;

    rightData.sense = getEncoderRight();
    leftData.sense = getEncoderLeft(); //get encoder readings
    //printf("\nsense%f.1", sense);
    
    if(runRight) {runRight = CalculatePID(rightData, target, pid);}
    if(runLeft) {runLeft = CalculatePID(leftData, target, pid);} 
    
    printf("Right: %d\n", rightData.error);
    printf("Left: %d\n", leftData.error);

    chassisSet(leftData.speed * leftData.turnMultiplier, rightData.speed * rightData.turnMultiplier);        //request the calculated motor speed
    tracking();    
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
