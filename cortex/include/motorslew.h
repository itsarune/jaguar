#ifndef _MOTORSLEW_H_
#define _MOTORSLEW_H_

//include relevant source files
#include "motorslew.h"
#include "main.h"      

//request motor speed for the slewing task to achieve
void motorReq(int channel, int speed);

void motorslewing(void * parameter);  //manages speed of motors

/*
 * PID Data controllers for motors
 * */
typedef struct pidData {
  float sense;
  double p,i,d;
  int lastError, errorLongTimeAgo;
  int integral;
  int error, derivative, speed;
  int target, lastTarget, timePassed;
} pidData;

/*
 * Reset PID Data for an individual motor
 * */
void Reset(pidData* data);

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
int pidData CalculatePID(pidData* data);

/*
 * Start motor-slewing process for motors
 * */
void motorslewing(void * parameter);

#endif
