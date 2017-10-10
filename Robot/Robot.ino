#include "Arduino.h"
#include "Messages.h"
#include <LiquidCrystal.h>
#include <Servo.h>
#include "LineSensor.h"
#include "Motor.h"
#include "Pinout.h"
#include "Drivetrain.h"
#include "Wire.h"
#include "States.h"

Drivetrain drive;
LineSensor lineSensor;

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

//Arm Servo variables
Servo jxservo;
Servo gripservo;

int pos = 10;
int posmin = 0;    // variable to store the servo position
int posmax = 500;  

void setup() {
  // put your setup code here, to run once:
  drive.DrivetrainSetup();

  // Andrew Added
  //LCD setup
  lcd.begin(16, 2);
  pinMode(DRIVEMODELED, OUTPUT);
  // SERVO SETUP
  jxservo.attach(JXSERVO);
  gripservo.attach(GRIPSERVO);

  Serial.begin(115200);
  Serial.println("Starting");
  msg.setup();
  timeForHeartbeat = millis() + 1000;
}

void loop() {
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
      msg.sendMessage(0x03); // Send radiation
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

  } else {
    //Serial.println("Autonomous");
    digitalWrite(DRIVEMODELED, HIGH); // Inverted logic
  }
}


/*  float topSpeed = 25.0;
  float multiplier = topSpeed*lineSensor.avgLinePos();

  drive.setPower(topSpeed + multiplier, topSpeed - multiplier);
  //lineSensor.test();

  Serial.println(multiplier/topSpeed);*/
