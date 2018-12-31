#include "tracking.h"
#include "encoder_pid.h"
#include <math.h>

/*
//------------ Version if it can run in parallel with the opcontrol ------------

float xpos = 0; //xpos, angle and ypos are global so they can be accessed anywhere
float ypos = 0;
float angle = 0;

const int encodermove = 100; //Encoder value to move the robot 1 in, depends on build
const int encoderturn = 10; //Encoder value to turn the robot 1 degree

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
      distance = encoderGet(encoderLeft)/encodermove;
      xpos += distance*sin(angle);
      ypos += distance*cos(angle);
      encoderReset(encoderRight);
      encoderReset(encoderLeft);
    }
    if(prevaction == 2)
    {
      distance = encoderGet(encoderLeft)/encodermove;
      xpos -= distance*sin(angle);
      ypos -= distance*cos(angle);
      encoderReset(encoderRight);
      encoderReset(encoderLeft);
    }
    if(prevaction == 3)
    {
      angle += encoderGet(encoderLeft)/encoderturn;
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

const float encodermove = 9.89/360; //Encoder value to move the robot 1 in, depends on build
const float encoderturn = 10; //Encoder value to turn the robot 1 degree

void tracking() {
float distance = 0;
int action = 1; //tracks what we're currently doing- 1 =fwd 2 = bck 3 = turn

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
    distance = encoderGet(encoderLeft)/encodermove;
    xpos += distance*sin(angle);
    ypos += distance*cos(angle);
    changeOffsets(encoderGet(encoderRight), encoderGet(encoderLeft));
    encoderReset(encoderRight);
    encoderReset(encoderLeft);
  }
  if(prevaction == 2)
  {
    distance = encoderGet(encoderLeft)/encodermove;
    xpos -= distance*sin(angle);
    ypos -= distance*cos(angle);
    changeOffsets(encoderGet(encoderRight), encoderGet(encoderLeft));
    encoderReset(encoderRight);
    encoderReset(encoderLeft);
  }
  if(prevaction == 3)
  {
    angle += encoderGet(encoderLeft)/encoderturn;
    changeOffsets(encoderGet(encoderRight), encoderGet(encoderLeft));
    encoderReset(encoderRight);
    encoderReset(encoderLeft);
  }
}

prevaction = action; //Update the previous action
}

//------------ EXPERIMENT. To be used with ball shooter. Not sure if it works ------------
void align() {
  //float requiredMovement = angle * encoderturn; //Calculate encoder ticks required to align straight

  if(angle <= 180) { //Check if the robot should turn left or right
    //encoderMotor(&driveStraightRight, &driveStraightLeft, requiredMovement, requiredMovement, false, true);
  }
  else {
    //encoderMotor(&driveStraightRight, &driveStraightLeft, requiredMovement, requiredMovement, true, false);
  }
}
