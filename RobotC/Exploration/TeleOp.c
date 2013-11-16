#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     irSensor,       sensorHiTechnicIRSeeker600)
#pragma config(Motor,  mtr_S1_C1_1,     rightDrive,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     leftDrive,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     gHook,         tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Tele-Operation Mode Code Template
//
// This file contains a template for simplified creation of an tele-op program for an FTC
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

int driveDamp = 25;

int armSpeed = 0;
int armAccelUp = 5;
int armAccelDown = 5;
int armStopUp = 7;
int armStopDown = 3;
int armDownMV = -20;
int armUpMV = 30;


int leftMotorTarget = 0;
int rightMotorTarget = 0;


bool armUp = false;
bool armDown = false;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of tele-op mode, you may want to perform some initialization on your robot
// and the variables within your program.
//
// In most cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{
  // Place code here to sinitialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

  return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the tele-op robot operation. Customize as appropriate for
// your specific robot.
//
// Game controller / joystick information is sent periodically (about every 50 milliseconds) from
// the FMS (Field Management System) to the robot. Most tele-op programs will follow the following
// logic:
//   1. Loop for'ever repeating the following actions:
//   2. Get the latest game controller / joystick settings that have been received from the PC.
//   3. Perform appropriate actions based on the joystick + buttons settings. This is usually a
//      simple action:
//      *  Joystick values are usually directly translated into power levels for a motor or
//         position of a servo.
//      *  Buttons are usually used to start/stop a motor or cause a servo to move to a specific
//         position.
//   4. Repeat the loop.
//
// Your program needs to continuously loop because you need to continuously respond to changes in
// the game controller settings.
//
// At the end of the tele-op periood, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void deadZone()
{

	if(abs(leftMotorTarget) < 10)
	{
		leftMotorTarget = 0;
	}

	if(abs(rightMotorTarget) < 10)
	{
		rightMotorTarget = 0;
	}

	/*if(abs(joystick.joy2_y1)<10)
		joystick.joy1_y1=0;
	if(abs(joystick.joy2_y2)<10)
		joystick.joy1_y2=0;*/
}

void inputManager()
{
	while(true)
	{
		getJoystickSettings(joystick);
		// Left/Right Stick
		// A/B Button
		// Gripper Mechanism

		leftMotorTarget = joystick.joy1_y1;
		rightMotorTarget = joystick.joy1_y2;

		if(joy1Btn(4) == 1)
		{
			armUp = true;
		}
		else
		{
			armUp = false;
			if(joy1Btn(1) == 1)
			{
				armDown = true;
			}
			else
			{
				armDown = false;
			}
		}
	}
}

int min(int a, int b)
{
	if(a > b)
	{
		return b;
	}
	else
	{
		return a;
	}
}

task drive()
{
	//writeDebugStream("int x is: %d \n", leftDrive);
	while(true)
	{
		deadZone();
		if(motor[rightDrive] != rightMotorTarget)
		{
			if(motor[rightDrive] < rightMotorTarget)
			{
				motor[rightDrive] += min(driveDamp, rightMotorTarget - motor[rightDrive]);
			}
			else
			{
				motor[rightDrive] -= min(driveDamp, motor[rightDrive] - rightMotorTarget);
			}
		}

		if(motor[leftDrive] != leftMotorTarget)
		{
			if(motor[leftDrive] < leftMotorTarget)
			{
				motor[leftDrive] += min(driveDamp, leftMotorTarget - motor[leftDrive]);
			}
			else
			{
				motor[leftDrive] -= min(driveDamp, motor[leftDrive] - leftMotorTarget);
			}
		}

		if(armUp && armSpeed < armUpMV)
		{
			motor[arm] += armAccelUp;
			armSpeed += armAccelUp;
		}
		else if(armDown && armSpeed > armDownMV)
		{
			motor[arm] -= armAccelDown;//(armDamp / 1.5);
			armSpeed -= armAccelDown;//(armDamp / 1.5);
		}

		if(!armDown && !armUp)
		{
			if(armSpeed > 0)
			{
				motor[arm] -= min(armSpeed, armStopUp);
				armSpeed -= min(armSpeed, armStopUp);
			}
			else if(armSpeed < 0)
			{
				motor[arm] += min(abs(armSpeed), armStopDown);
				armSpeed += min(abs(armSpeed), armStopDown);
			}

		}

		wait10Msec(5);

	}
}

task moveArm()
{



	/*
		while (motor[arm] < MAX_SPEED)
		{
			motor[motorA] += RATE_OF_CHANGE;
			wait10Msec;
		}
	*/
}

task main()
{
  initializeRobot();

	StartTask(drive);
	StartTask(moveArm);
	inputManager();


}
