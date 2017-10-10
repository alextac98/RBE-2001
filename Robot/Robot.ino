
#include "LineSensor.h"
#include "Motor.h"
#include "Pinout.h"
#include "Drivetrain.h"
#include "Wire.h"

Drivetrain drive;
LineSensor lineSensor;

#define uchar unsigned char
uchar t;
//void send_data(short a1,short b1,short c1,short d1,short e1,short f1);
uchar data[16];

void setup() {
  // put your setup code here, to run once:
	drive.DrivetrainSetup();
	Serial.begin(9600);

}

void loop() {
	

}

/*  float topSpeed = 25.0;
  float multiplier = topSpeed*lineSensor.avgLinePos();

  drive.setPower(topSpeed + multiplier, topSpeed - multiplier);
  //lineSensor.test();

  Serial.println(multiplier/topSpeed);*/
