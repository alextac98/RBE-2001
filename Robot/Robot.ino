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
int lastTime;

float topSpeed =30.0;
float multiplier;

void setup() {
  // put your setup code here, to run once:
  drive.DrivetrainSetup();
  arm.armSetup();

  // Andrew Added
  //LCD setup
  lcd.begin(16, 2);
  pinMode(DRIVEMODELED, OUTPUT);

  pinMode(TUBESENSOR, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Starting");
  msg.setup();
  timeForHeartbeat = millis() + 1000;
  robotDo = Start;
}

void loop() {

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
			drive.setPower(-topSpeed, -topSpeed);
			if (lineSensor.isAllBlack()) {
				Serial.println("all black");
				counter++;
				drive.setPower(-topSpeed*1.25, topSpeed*1.25);
			}
			break;
		case 1:
			multiplier = topSpeed*1.25*lineSensor.avgLinePos();
			drive.setPower(-topSpeed*1.25 + multiplier, topSpeed*1.25 - multiplier);
			if (lineSensor.isCentered()) {
				robotDo = findUsedDispenser;
				counter = 0;
			}
			break;
		default:
			break;
		}
		break;
	case findUsedDispenser:
		multiplier = topSpeed*lineSensor.avgLinePos();
		drive.setPower(topSpeed + multiplier, topSpeed - multiplier);
		break;
	default:
		drive.setPower(0, 0);
		break;
	}

	//fourBar.setPosition(up);
	//drive.setPower(0, 0);
	//bluetoothComs();
	//float topSpeed = 50.0;
	//float multiplier = topSpeed*lineSensor.avgLinePos();

	//drive.setPower(topSpeed + multiplier, topSpeed - multiplier);
}

void bluetoothComs() {
	if (msg.readcomms()) { msg.printMessage(); }
	if (millis() > timeForHeartbeat)
	{
		timeForHeartbeat = millis() + 1000;
		// msg.setrobotmovestate(0x01)
		//if (robotstatuscounter == 0) // If Now is the time so send the counter
		if (radcounter == 0)
		{
			msg.sendHeartbeat();
			msg.setradAlert(spentfuel); // new fuel alert = 0xFF, spent fuel = 0x2C
			msg.sendMessage(kRadiationAlert); // Send radiation
			radcounter = (radcounter + 1) % 4; // this equals either 0, 1, or 2.
		}
		else
		{
			msg.sendHeartbeat();
			radcounter = (radcounter + 1) % 4; // this equals either 0, 1, or 2.
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

	if (msg.isStopped())
	{
		//Serial.println("Stopped");
		digitalWrite(DRIVEMODELED, LOW); // Inverted logic

	}
	else {
		//Serial.println("Autonomous");
		digitalWrite(DRIVEMODELED, HIGH); // Inverted logic
	}
}