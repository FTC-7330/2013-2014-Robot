#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     irSensor,       sensorHiTechnicIRSeeker600)
#pragma config(Motor,  motorA,          flag,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     rightDrive,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     leftDrive,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     arm,           tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     winch,         tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    gripper,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    safety,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    trigger,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c" .

int driveDamp = 25;

int leftMotorTarget = 0;
int rightMotorTarget = 0;
int gripperOpenPos = 240;
int gripperClosedPos = 85;

bool armTurbo = false;
bool armForward = false;
bool armBackward = false;
bool gripperOpen = false;

bool flagRunning = false;
bool winchIn = false;



void initializeRobot()
{
	nMotorEncoder[leftDrive] = 0;
	nMotorEncoder[rightDrive] = 0;
	gripperOpen = false;
  return;
}


/* inputManager reads input from both controllers and sets the values of
	 corresponding variables.

	 Button 1- A
	 Button 2- B
	 Button 3- X
	 Button 4- Y
	 Button 5- LB
	 Button 6- RB

	 Controller 1:
	 		Left Joystick - Left Motor (leftMotorTarget)
	 		Right Joystick - Right Motor (rightMotorTarget)
	 		Left Button - Flag Raiser Counterclockwise
	 		Right Button - Flag Raiser Clockwise


	 	Controller 2:
	 		B - Opens or closes gripper (gripperOpen)
	 		DPad Up - Arm up (armForward)
	 		DPad Down - Arm down (armBackward)
	 		Left Button - Arm turbo (armTurbo)

*/
void inputManager()
{

	bool buttonBIsDown = false;
	bool buttonBWasDown = false;

	while(true)
	{
		getJoystickSettings(joystick);

		if(abs(joystick.joy1_y2) < 10)
		{
			leftMotorTarget = 0;
		}
		else
		{
			leftMotorTarget = joystick.joy1_y2;
		}

		if(abs(joystick.joy1_y1) < 10)
		{
			rightMotorTarget = 0;
		}
		else
		{
			rightMotorTarget = joystick.joy1_y1;
		}

		armTurbo = (joy2Btn(5) == 1);
		armForward = (joystick.joy2_TopHat == 0);
		armBackward = (joystick.joy2_TopHat == 4);

		flagRunning = (joy1Btn(5) == 1);

		if((joy1Btn(6) == 1)&&(joy2Btn(6)==1))
		{
			winchIn=true;
		}
		else
		{
			winchIn = false;
		}

		buttonBIsDown = (joy2Btn(2) == 1);
		if(buttonBIsDown && !buttonBWasDown)
		{
			gripperOpen = !gripperOpen;
		}
		buttonBWasDown = buttonBIsDown;
	}
}

// min takes in two integers as paramaters and returns the smaller of the two
int min(int a, int b)
{
	if(a > b)
	{
		return b;
	}
    return a;
}

// drive sets the values of the drive motors
task Drive()
{
	bool leftEncoderChanged = true;
	bool rightEncoderChanged = true;
	int pastLeftEncoder;
	int pastRightEncoder;

	while(true)
	{
	//int leftEncoder = nMotorEncoder[leftDrive];
	//int rightEncoder = nMotorEncoder[rightDrive];

	//	if(leftEncoder!=pastLeftEncoder)
	//	{
	//		leftEncoderChanged = true;
	//	}
	//	else
	//	{
	//		leftEncoderChanged = false;
	//	}

	//	if(rightEncoder!=pastRightEncoder)
	//	{
	//		rightEncoderChanged = true;
	//	}
	//	else
	//	{
	//		rightEncoderChanged = false;
	//	}

		/*if((!rightEncoderChanged&&abs(motor[rightDrive])>10)||(!leftEncoderChanged&&abs(motor[leftDrive])>10))
		{
			motor[rightDrive] = 0;
			motor[leftDrive] = 0;
			//PlaySound(soundBeepBeep);
			wait10Msec(10);
			ClearSounds();
		}*/

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
		//pastLeftEncoder = leftEncoder;
		//pastRightEncoder = rightEncoder;
		}

		wait10Msec(5);

	}
}

// arm sets the value of the arm motor
task Arm()
{
	while(true)
	{
		if(armBackward)
		{
			if(armTurbo)
			{
				motor[arm] = -30;
			}
			else
			{
				motor[arm] = -3;
			}
		}
		else if(armForward)
		{
			if(armTurbo)
			{
				motor[arm]= 30;
			}
			else
			{
				motor[arm]= 3;
			}
		}
		else
		{
			motor[arm]=0;
		}

		wait10Msec(5);
	}
}

// gripper changes the servo value of the gripper
task Gripper()
{
	while(true)
	{
		if(!gripperOpen)
		{
			servo[gripper] = gripperClosedPos;
		}

		if(gripperOpen)
		{
			servo[gripper] = gripperOpenPos;
		}
	}
	wait10Msec(5);
}

// flagRaiser sets the value of the flag raising motor
task FlagRaiser()
{
	while(true)
	{
		if(flagRunning)
		{
			motor[flag] = 100;
		}
		else
		{
			motor[flag] = 0;
		}
		wait10Msec(5);
	}
}

task Winch()
{
	while(true)
	{
		if(winchIn)
		{
			motor[winch] = -60;
		}
		else
		{
			motor[winch] = 0;
		}

		wait10Msec(5);
	}
}

task main()
{
  initializeRobot();

  waitForStart();

	StartTask(Drive);
	StartTask(Arm);
	StartTask(Gripper);
	StartTask(FlagRaiser);
	StartTask(Winch);

  inputManager();

}
