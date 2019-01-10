#include "tracking.h"
#include "encoder_pid.h"
#include <math.h>

/*
//------------ Version if it can run in parallel with the opcontrol ------------

float xpos = 0; //xpos, angle and ypos are global so they can be accessed anywhere
float ypos = 0;
float angle = 0;

const int encoderMoveConstant = 100; //Encoder value to move the robot 1 in, depends on build
const int encoderTurnConstant = 10; //Encoder value to turn the robot 1 degree

void tracking() {
int action = 1; //tracks what we're currently doing- 1 =fwd 2 = bck 3 = turn
int prevaction = 1;
float distance = 0;


//While moving forwards
while(1)
{
  prevaction = action;
  //motorGet checks the speed assigned to the motors. If this is running in parallel to the main opcontrol
  //then it will not be possible to get power and turn, therefore this must be used
  if(motorGet(leftMotor1) > 0 && motorGet(rightMotor1) > 0) //assuming old joystick mapping, will change depending on how we go fwd
  {
    action = 1;
  }

  //While moving backwards
  if(motorGet(leftMotor1) < 0 && motorGet(rightMotor1) < 0)
  {
    action = 2;
  }

  //While Turning
  else
  {
   action = 3;
  }

  if(prevaction != action) //this if statement will be true when we change what we're doing e.g going from forwards to turning
  //when this happens, we record values from the previous action, eg how far we turned/went forwards
  {
    if(prevaction == 1)
    {
      distance = encoderGet(encoderLeft)/encoderMoveConstant;
      xpos += distance*sin(angle);
      ypos += distance*cos(angle);
      encoderReset(encoderRight);
      encoderReset(encoderLeft);
    }
    if(prevaction == 2)
    {
      distance = encoderGet(encoderLeft)/encoderMoveConstant;
      xpos -= distance*sin(angle);
      ypos -= distance*cos(angle);
      encoderReset(encoderRight);
      encoderReset(encoderLeft);
    }
    if(prevaction == 3)
    {
      angle += encoderGet(encoderLeft)/encoderTurnConstant;
      encoderReset(encoderRight);
      encoderReset(encoderLeft);
    }
  }
}

}
*/

//------------ Version if it is called every cycle ------------
float xpos = 0; //variables need to be global so that they do not get redefined every function call
float ypos = 0;
int prevaction = 1;
float angle = 0;
int action = 1; //tracks what we're currently doing- 1 =fwd 2 = bck 3 = turn

//Robot has 4 inch diameter wheels
const float encoderMoveConstantCM = 11.279; //Encoder value to move the robot 1 cm
const float encoderMoveConstant = 28.648; //Encoder value to move the robot 1 in, depends on build

const float encoderTurnConstant = 4.022; //Encoder value to point turn the robot 1 degree
const float encoderWideTurnConstant = 8.044;

void tracking()
{
  float distance = 0;

  //motorGet checks the speed assigned to the motors. If this is running in parallel to the main opcontrol
  //then it will not be possible to get power and turn, therefore this must be used
  //While moving straight
  int leftSpeed = motorGet(leftMotor1);
  int rightSpeed = -motorGet(rightMotor1);

  if (millis() % 500 < 3)
  {
    printf("X: %f\n Y: %f\n", xpos, ypos);
    printf("angle: %f\n", angle);
  }

  int leftJoystickValue = 0;
  if(joystickGetAnalog(1, 3) > 25)
  {
    leftJoystickValue = joystickGetAnalog(1, 3);
  }
  int rightJoystickValue = 0;
  if(joystickGetAnalog(1, 2) > 25)
  {
    rightJoystickValue = joystickGetAnalog(1, 2);
  }

  if ((leftJoystickValue > 0 && rightJoystickValue > 0) || (leftJoystickValue < 0 && rightJoystickValue < 0))
  {
    printf("straight\n");
    action = 1;
  }

  //While doing 1 wheel turn
  else if ((leftJoystickValue == 0 && abs(rightJoystickValue) > 0) || (abs(leftJoystickValue) > 0 && rightJoystickValue == 0))
  {
    printf("One wheel\n");
    action = 2;
  }

  //While doing point turn (2 wheel turn)
  else if ((leftJoystickValue > 0 && rightJoystickValue < 0) || (leftJoystickValue < 0 && rightJoystickValue > 0))
  {
    printf("2wheel\n");
    action = 3;
  }

  if (prevaction != action) //this if statement will be true when we change what we're doing e.g going from forwards to turning
  //when this happens, we record values from the previous action, eg how far we turned/went forwards
  {
    if (prevaction == 1) //Straight
    {
      distance = encoderGet(encoderLeft) / encoderMoveConstant;
      xpos += distance * sin(angle);
      ypos += distance * cos(angle);
      changeOffsets(encoderGet(encoderRight), encoderGet(encoderLeft));
      encoderReset(encoderRight);
      encoderReset(encoderLeft);
    }
    if (prevaction == 2)
    {
      if (abs(leftSpeed) > abs(rightSpeed))
      {
        angle += encoderGet(encoderLeft) / encoderWideTurnConstant;
      }
      else
      {
        angle += encoderGet(encoderRight) / encoderWideTurnConstant;
      }
      angle = (int)angle%360;
      if(angle < 0) {
        angle = 360 + angle;
      }
      changeOffsets(encoderGet(encoderRight), encoderGet(encoderLeft));
      encoderReset(encoderRight);
      encoderReset(encoderLeft);
    }
    if (prevaction == 3)
    {
      angle += encoderGet(encoderLeft) / encoderTurnConstant;
      angle = (int)angle%360;
      if(angle < 0) {
        angle = 360 + angle;
      }
      changeOffsets(encoderGet(encoderRight), encoderGet(encoderLeft));
      encoderReset(encoderRight);
      encoderReset(encoderLeft);
    }
  }

  prevaction = action; //Update the previous action
}

//------------ EXPERIMENT. To be used with ball shooter. Not sure if it works ------------
void align() {
  //float requiredMovement = angle * encoderTurnConstant; //Calculate encoder ticks required to align straight
  if(angle < 0) {
    angle = 360 + angle;
  }
  if(angle <= 180) { //Check if the robot should turn left or right
    //encoderMotor(&driveStraightRight, &driveStraightLeft, requiredMovement, requiredMovement, false, true);
  }
  else {
    //encoderMotor(&driveStraightRight, &driveStraightLeft, requiredMovement, requiredMovement, true, false);
  }
}
