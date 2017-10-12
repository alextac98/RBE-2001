#include "Arm.h"
#include "Arduino.h"
#include "Messages.h"
#include <LiquidCrystal.h>
#include <Servo.h>
#include "LineSensor.h"
#include "Pinout.h"
#include "Drivetrain.h"
#include "Wire.h"
#include "States.h"

Drivetrain drive;
LineSensor lineSensor;
Arm arm;

//FourBar fourBar;

#define uchar unsigned char
uchar t;
//void send_data(short a1,short b1,short c1,short d1,short e1,short f1);
uchar data[16];

// Andrew Added
Messages msg;                                                           // The message object to read in commands from reactor control.
unsigned long timeForHeartbeat;                                         // Used for timing the heartbeat.

LiquidCrystal lcd(DISPLAY1, DISPLAY2, DISPLAY3, DISPLAY4, DISPLAY5, DISPLAY6);

unsigned int radcounter = 0;                                            // This keeps the timing of the radiation message every 2! seconds.
unsigned int robotstatuscounter = 0;                                    // This is for delaying the robot status messages less often than 5 seconds in between each message.

int pos = 10;
int posmin = 0;    // variable to store the servo position
int posmax = 500;  

robotdoState robotDo;
pickupState armDo;
int counter = 0;
int prevCounter = 0;
int lastTime;

int storetubeavailability = 0;
int supplytubeavailability = 0;

float topSpeed =30.0;
float turningSpeed = 55.0;
float multiplier;

bool stopnow = false;
bool wasstopped = false;

robotdoState prevState;

void setup() {
  // put your setup code here, to run once:
  drive.DrivetrainSetup();
  arm.armSetup();

  // Andrew Added
  //LCD setup
  lcd.begin(16, 2);
  pinMode(DRIVEMODELED, OUTPUT);

  pinMode(TUBESENSOR, INPUT_PULLUP);

  Serial.begin(14400);
  Serial.println("Starting");
  msg.setup();
  timeForHeartbeat = millis() + 1000;
  robotDo = Start;
  prevState = robotDo;
  msg.setrobotmovestate(stopped);
  msg.setradAlert(nofuel);
}

void loop() {
	bluetoothComs();
	stopnow = msg.isStopped();
	if (stopnow && wasstopped == false) {
		wasstopped = true;
		prevState = robotDo;
		prevCounter = counter;
		robotDo = robotStop;
	}
	else if (stopnow == false && wasstopped == true) {
		wasstopped = false;
		robotDo = prevState;
		counter = prevCounter;
	}

	switch (robotDo)
	{
	case Start:
		//Set arm up
		arm.setArmPosition(up);
		// open gripper
		arm.openGripper();
		//drive motors set to zero
		drive.setPower(0, 0);
		if (arm.isArmPosition(up)) {
			robotDo = approachNuke;
		}
		
		break;
	case approachNuke:
		if (digitalRead(TUBESENSOR)) {
			drive.setPower(0, 0);
			robotDo = pickUpNukeLow;
			armDo = armDownward;
		}
		else {
			multiplier = topSpeed*lineSensor.avgLinePos();
			drive.setPower(topSpeed + multiplier, topSpeed - multiplier);
		}
		break;
	case pickUpNukeLow:
		//pick up nuke
		drive.setPower(0, 0);
		switch (armDo)
		{
		case armDownward:
			arm.setArmPosition(down);
			if (arm.isArmPosition(down)) {
				armDo = close;
			}
			break;
		case close:
			if (arm.closeGripper()) {
				armDo = armUpward;
				msg.setradAlert(spentfuel);
			}
			break;
		case armUpward:
			//Serial.println("hi");
			arm.setArmPosition(up);
			if (arm.isArmPosition(up)) {
				robotDo = goToCenter;
				counter = 0;
				Serial.println("next state");
			}
			break;
		default:
			break;
		
		}
		break;
	case goToCenter:
		//go to center
		switch (counter)
		{
		case 0:
			Serial.println("backing up");
			drive.setPower(-topSpeed, -topSpeed);
			if (lineSensor.isAllBlack()) {
				Serial.println("all black");
				counter++;
				// drive.setPower(turningSpeed, -turningSpeed);
			}
			break;
		case 1:
			Serial.println("starting to turn");
			drive.setPower(turningSpeed, -turningSpeed);
			if (lineSensor.sendProcessedValue(9) == 1) {
				counter++;
			}
			break;
		case 2:
			drive.setPower(turningSpeed, -turningSpeed);
			//Serial.println("Finishing to turn");
			if (lineSensor.sendProcessedValue(6) == 1){
				counter++;
			}
			break;
		case 3:
			Serial.println("line following");
			multiplier = topSpeed*lineSensor.avgLinePos();
			drive.setPower(topSpeed + multiplier, topSpeed - multiplier);
			break;
		default:
			//drive.setPower(0, 0);
			Serial.println("why am I here");
		}

	case findUsedDispenser:

		storetubeavailability = msg.whichStore();
		drive.setPower(0, 0);

		/*int storetubeavailability = 0;
int supplytubeavailability = 0;*/

		//multiplier = topSpeed*lineSensor.avgLinePos();
		//drive.setPower(topSpeed + multiplier, topSpeed - multiplier);
		break;
	
	case robotStop:
		drive.setPower(0, 0);
		if (!msg.isStopped()) {
			robotDo = prevState;
			break;
		}
		else {
			robotDo = robotStop;
		}
		break;
	default:
		drive.setPower(0, 0);
		break;
	}

}



void bluetoothComs() {
	if (msg.readcomms()) { 
		//msg.printMessage(); 
	}
	if (millis() > timeForHeartbeat)
	{
		timeForHeartbeat = millis() + 1000;
		// msg.setrobotmovestate(0x01)
		//if (robotstatuscounter == 0) // If Now is the time so send the counter
		if (radcounter == 0)
		{
			msg.sendHeartbeat();

			// msg.setradAlert(spentfuel); // new fuel alert = 0xFF, spent fuel = 0x2C
			msg.sendMessage(kRadiationAlert); // Send radiation
			radcounter = (radcounter + 1) % 4; // this equals either 0, 1, or 2.
		}
		else
		{
			msg.sendHeartbeat();
			radcounter = (radcounter + 1) % 4; // this equals either 0, 1, 2, or 3.
		}
		robotstatuscounter = (robotstatuscounter + 1) % 6; // This is for the robot counter. Let's make this a seperate if statement.
	}

	lcd.setCursor(0, 0);
	lcd.print("Supply");
	lcd.setCursor(7, 0);
	lcd.print(msg.readsupply(), HEX);
	lcd.setCursor(0, 1);
	lcd.print("Stored");
	lcd.setCursor(7, 1);
	lcd.print(msg.readstorage(), HEX);


	if (msg.readradAlert() == spentfuel)
	{
		analogWrite(DRIVEMODELED, 512);
	}
	else if (msg.readradAlert() == newfuel)
	{
		analogWrite(DRIVEMODELED, 256);
	}
	else {
		analogWrite(DRIVEMODELED, 0);
	}
	/*
	if (msg.isStopped())
	{
		//Serial.println("Stopped");
		digitalWrite(DRIVEMODELED, LOW); // Inverted logic

	}
	else {
		//Serial.println("Autonomous");
		digitalWrite(DRIVEMODELED, HIGH); // Inverted logic
	}
	*/

}
