#include "PC_FileIO.c"

const int MAX_SIZE = 30;

//welcome message
void welcomeMessage()
{
	displayCenteredBigTextLine(2, "WELCOME");
	displayCenteredBigTextLine(4, "TO");
	displayCenteredBigTextLine(6, "SIMON SAYS!");
	displayCenteredBigTextLine(8, "860");

	displayString(14, "  PRESS ANY BUTTON TO BEGIN...");

	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonAny))
	{}
	eraseDisplay();
}

//displays current level
void levelCompletion(int level)
{
  eraseDisplay();
  displayCenteredBigTextLine(5,"Level: %d",level);
}

//updates mistake counter
void isGameStillGoingOn (int * input, int * output, int & mistakeCounter)
{
	for (int index = 0; index < MAX_SIZE && mistakeCounter == 0; index++)
	{
		if(input[index] == output[index])
			mistakeCounter = 0;
		else
			mistakeCounter = 1;	
	}	
}

//returns which button the user pressed
int touchInput(tSensors port1, tSensors port2, tSensors port3, tSensors port4)
{
	wait1Msec(150);
	int x = -1;
 	while (SensorValue[port1] == 0 && SensorValue[port2] == 0 && SensorValue [port3] == 0 && SensorValue [port4] == 0)
 	{}
 	if(SensorValue[port1] == 1)
 		x = 0;
 	else if (SensorValue[port2] == 1)
 		x = 1;
 	else if (SensorValue[port3] == 1)
 		x = 2;
 	else
 		x = 3;
 	while (SensorValue[port1] == 1 || SensorValue[port2] == 1 || SensorValue [port3] == 1 || SensorValue [port4] == 1)
 	{}
 	return x;
}

//raises respective flag
void raiseFlag(int outputNumber)
{
	const int ENC_LIMIT = 30;
	const int MOTORSPEED = 10;

	if(outputNumber == 0)
	{
		nMotorEncoder[motorA] = 0;
		wait1Msec(50);
		motor[motorA] = MOTORSPEED;
		while (nMotorEncoder[motorA] < ENC_LIMIT)
		{}
	 	motor[motorA] = 0;
	 	wait1Msec(1000);
	 	motor[motorA] = -MOTORSPEED;
	 	while(nMotorEncoder[motorA] > 0)
	 	{}
	 	motor[motorA] = 0;
	 }

	 else if(outputNumber == 3)
	 {
	 	nMotorEncoder[motorD] = 0;
		wait1Msec(50);
		motor[motorD] = MOTORSPEED;
		while (nMotorEncoder[motorD] < ENC_LIMIT)
		{}
	 	motor[motorD] = 0;
	 	wait1Msec(1000);
	 	motor[motorD] = -MOTORSPEED;
	 	while(nMotorEncoder[motorD] > 0)
	 	{}
	 	motor[motorD] = 0;
	 }

	 else if(outputNumber == 1)
	 {
	 	nMotorEncoder[motorA] = 0;
		wait1Msec(50);
		motor[motorA] = -MOTORSPEED;
		while (nMotorEncoder[motorA] > -ENC_LIMIT)
		{}
	 	motor[motorA] = 0;
	 	wait1Msec(1000);
	 	motor[motorA] = MOTORSPEED;
	 	while(nMotorEncoder[motorA] < 0)
	 	{}
	 	motor[motorA] = 0;
	 }

	else if(outputNumber == 2)
	 {
	 	nMotorEncoder[motorD] = 0;
		wait1Msec(50);
		motor[motorD] = -MOTORSPEED;
		while (nMotorEncoder[motorD] > -ENC_LIMIT)
		{}
	 	motor[motorD] = 0;
	 	wait1Msec(1000);
	 	motor[motorD] = MOTORSPEED;
	 	while(nMotorEncoder[motorD] < 0)
	 	{}
	 	motor[motorD] = 0;
  }
}

//highscore update
void makesHighscore(int userScore)
{
	TFileHandle fin;
	bool fileOpened = openReadPC(fin, "scores.txt");

  if (!fileOpened)
  {
    displayString(5, "Cannot open scores");
    wait1Msec(5000);
	}

	int playerNumber[5]={0,0,0,0,0};
	int topScores[5]={0,0,0,0,0};
	int index = 0;
  int currentPlayerNumber = 0;
	readIntPC(fin,currentPlayerNumber);
	for (int count = 0; count <=4; count++)
	{
		readIntPC(fin, playerNumber[count]);
    readIntPC(fin, topScores[count]);
	}
	while ((index<5)&&(userScore < topScores[index]))
	{
		index++;
	}
	if (userScore >= topScores[index])
	{
		for(int count = 4; count > index; count--)
		{
			topScores[count] = topScores[count-1];
			playerNumber[count] = playerNumber[count-1];
		}
		topScores[index] = userScore;
		playerNumber[index] = currentPlayerNumber+1;
	}
	closeFilePC(fin);

	TFileHandle fout;
 	bool okay = openWritePC(fout, "scores.txt");
 	if (!okay)
  {
    displayString(5, "Cannot write scores");
    wait1Msec(5000);
	}
	writeLongPC(fout,currentPlayerNumber+1);
	writeTextPC(fout, " ");
	for (int count = 0; count <=4; count++)
	{
		writeLongPC(fout, playerNumber[count]);
		writeTextPC(fout, " ");
		writeLongPC(fout, topScores[count]);
		writeTextPC(fout, " ");
	}
	closeFilePC(fout);
}

//shutdown procedure
void shutDown(int userScore)
{
	eraseDisplay();
	TFileHandle fin;
	bool fileOpened = openReadPC(fin, "scores.txt");

	if (!fileOpened)
	{
    displayString(5, "Cannot open scores");
    wait1Msec(5000);
	}

	int playerNumber[5] = {0,0,0,0,0};
	int topScores[5] = {0,0,0,0,0};
	int currentPlayerNumber = 0;

	readIntPC(fin, currentPlayerNumber);
	for (int count = 0; count <=4; count++)
	{
		readIntPC(fin, playerNumber[count]);
		readIntPC(fin, topScores[count]);
	}
	displayCenteredBigTextLine(2, "Your score: %d", userScore);
	displayCenteredBigTextLine(4, "Player#: %d", currentPlayerNumber);
	int count = 0;
	displayString(count+7, "        PLAYER  SCORE");
	for (count = 0; count<=4; count++)
		displayString(count+8, "           %d     %d", playerNumber[count], topScores[count]);

	displayString(14, " PRESS ANY BUTTON TO CONTINUE...");
	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonAny))
	{}
	eraseDisplay();
}

task main()
{
	SensorType[S1] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S2] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S4] = sensorEV3_Touch;
	wait1Msec(50);


  int input[MAX_SIZE] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
  												10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,};
  int output[MAX_SIZE] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
  												10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,};

  welcomeMessage();

  int mistakeCounter = 0, counter = 0;

	while(mistakeCounter == 0)
	{
		levelCompletion(counter + 1);
		output[counter] = random(3);
		for (int index0 = 0; index0 <= counter; index0++)
			raiseFlag(output[index0]);
		for (int index1 = 0; index1 <= counter; index1++)
		{
				input[index1] = touchInput(S1, S2, S3, S4);
		}
		isGameStillGoingOn(input, output, mistakeCounter);
		wait1Msec(1000);
		counter++;
	}

	makesHighscore(--counter);
	shutDown(counter);
}