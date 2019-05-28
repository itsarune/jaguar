#ifndef _MOTORSLEW_H_
#define _MOTORSLEW_H_

//include relevant source files
#include "motorslew.h"
#include "main.h"      

/* PID Data structure*/
typedef struct pidData {
  float sense;
  double p,i,d;
  int lastError, errorLongTimeAgo;
  int integral;
  int error, derivative, speed;
  int straightError, straightDerivative, straightLastError, straightLastTarget, straightIntegral;
  int target, lastTarget, timePassed;
  bool driveMotorLeft;
  bool driveMotorRight;
  bool isSlave;
  bool isLeft, driveAdjust;
  int straightTarget;
  bool straightDriveLeft, straightDriveRight, straightDriveAdjust;
} pidData;

pidData driveStraightRight, driveStraightLeft, driveTurnRight, driveTurnLeft;
pidData autonStraightRight, autonStraightLeft, autonBackRight, autonBackLeft;

//request motor speed for the slewing task to achieve
void motorReq(int channel, int speed);

/*
 * Set the slave drive motors
 * */
void setSlave(int slaveMotors[4]);

/*  
 *  Setting PID constants:
 *  (pointer to pid data structure, p const, i const, d const)
 *  */
void setConst(pidData * pid, float p, float i, float d);

void motorslewing(void * parameter);  //manages speed of motors

/*
 * Reset PID Data for an individual motor
 * */
void Reset(pidData* data);

/*  Gets value of the left encoder  */
int getEncoderLeft();

/*  Gets value fo the right encoder */
int getEncoderRight();

/*
  Begin PID movement for motor, by setting motor channel and motor target. 
*/
void beginPID(int motor, int target);

/*
 * Reset PID data for all motor controllers
 * */
void resetPID();

/*
 * Calculate PID data based on error
 * */
int CalculatePID(pidData* data);

/*
 * Start motor-slewing process for motors
 * */
void motorslewing(void * parameter);

/*  Request PID motor speed
 *  Pass motor channel and requested speed
 *  */
void motorReq(int channel, int speed);

/* Find ratio for turns */
void intRatio(int encoderTicks, int angle);

/*  Input angle for encoder-based turns */
void encoderTurn(float angle);

/*  change offsets for the encoder movements  */
void changeOffsets(int right, int left);
#endif
