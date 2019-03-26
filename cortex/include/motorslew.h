#ifndef _MOTORSLEW_H_
#define _MOTORSLEW_H_

//include relevant source files
#include "motorslew.h"
#include "main.h"      

//request motor speed for the slewing task to achieve
void motorReq(int channel, int speed);

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

#endif
