
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
  
	int *raw = lineSensor.processedArray();

	for (int i = 0; i <= 9; i++) {
		Serial.print("Pin "); Serial.print(i); Serial.print(" reads: ");
		Serial.println(raw[i]);
	}
	Serial.println();

	delay(1000);
}
