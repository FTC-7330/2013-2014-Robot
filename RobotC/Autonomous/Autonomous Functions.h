#pragma config(Sensor, S1,     touch,          sensorTouch)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     light,          sensorLightActive)
#pragma config(Sensor, S4,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,          gripperMotor,  tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,          rightMotor,    tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,          leftMotor,     tmotorNXT, openLoop, encoder)

// competition value: 20
// test value: 23
const int encoderTicksPer10Degrees = 260;
const int sonarDistance = 0;
const int armRaiseDistance = 1700;
const int armRaiseSpeed = 40;
const int rollingAvgWindow = 5;
const int rollingAvgInterval = 50;

// displays value of the IR sensor, the right and left encoders, and the sonar sensor
task display()
{
    while (true)
    {
    	//Displays information about encoders and IR and sonar sensors
			int display = SensorValue[irSensor];
			int right = nMotorEncoder[rightDrive];
			int left = nMotorEncoder[leftDrive];
			int sonarValue = SensorValue[sonar];

			nxtDisplayString(0,"%d", display);
			nxtDisplayString(1, "Right: %d", right);
			nxtDisplayString(2, "Left: %d", left);
			nxtDisplayString(3, "Sonar: %d", sonarValue);
	  }
}
//continues driving until encoders reach destination, then resets motor speed.
void waitForStop()
{
	while(nMotorRunState[rightDrive] != runStateIdle && nMotorRunState[leftDrive] != runStateIdle)
	{
	}

	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
}

//resets encoder values; if inputed degree value is positive robot turns right, if negative then the robot turns left.
void Turn(int degrees)
{
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive] = 0;

	nMotorEncoderTarget[rightDrive] = -degrees*encoderTicksPer10Degrees / 10;
	nMotorEncoderTarget[leftDrive] = degrees*encoderTicksPer10Degrees / 10;

	if (degrees < 0)
	{
		motor[rightDrive] = 90;
		motor[leftDrive] = -90;
  }
  else
  {
		motor[rightDrive] = -90;
		motor[leftDrive] = 90;
  }

	waitForStop();

	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive] = 0;
}
// reset encoder values; drives for the distance given at the given speed;
//if runForever is true, drives forever
void drive(int distanceRight, int distanceLeft, int speed, bool runForever)
{
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive] = 0;


	if (!runForever) //sets encoder targets if not running forever
	{
		nMotorEncoderTarget[rightDrive] = distanceRight;
		nMotorEncoderTarget[leftDrive] = distanceLeft;
	}
	else
	{
		nMotorEncoderTarget[rightDrive] = 0;
		nMotorEncoderTarget[leftDrive] = 0;
	}

	motor[rightDrive] = speed;
	motor[leftDrive] = speed;

	if (!runForever) //stops when target is reached.
	{
		waitForStop();
	}

	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive] = 0;
}

// drives arm for given distance for given speed
void driveArm(int distanceUp, int speed)
{
	nMotorEncoder[arm] = 0;
	nMotorEncoderTarget[arm] = distanceUp;
	motor[arm] = speed;

	while(nMotorRunState[arm]!=runStateIdle)
	{
	}
	motor[arm] = 0;

	nMotorEncoder[arm] = 0;
}


//raises arm as a task, allowing it to be done while driving forwards
// calls driveArm method as a task with parameters defined as constants
task armRaise()
{
	driveArm(armRaiseDistance, armRaiseSpeed);
}

task driveFailSafe()
{
	int encoderChange[rollingAvgWindow] = {0, 0, 0, 0, 0};
	int index = 0;
	int oldEncoderValue = nMotorEncoder[leftDrive];
	int oldPower = motor[leftDrive];
	wait1Msec(rollingAvgInterval);

	while(true)
	{
		int newEncoderValue = nMotorEncoder[leftDrive];
		int newPower = motor[leftDrive];

		if (oldPower != newPower)
		{
			for (int i = 0; i < rollingAvgWindow
		}

		encoderChange[index] = oldEncoderValue - newEncoderValue;
		if (index < 5)
	  {
			index++;
		}
		else
		{
			index = 0;
	  }

  	oldEncoderValue = newEncoderValue;
  	oldPower = newPower;
  	wait1Msec(rollingAvgInterval);
	}
}
