#include "LineSensor.h"

#include "Arduino.h"
#include <Wire.h>
#include "Pinout.h"

LineSensor::LineSensor(){
	Wire.begin();
}

void LineSensor::calibrate(){

}

void LineSensor::calibrate_show(){
	
}

void LineSensor::calibrate_reset(){
	
}

void LineSensor::test()
{
	unsigned int* raw = rawArray();
	
	for (int i = 0; i <= 9; i++) {
		Serial.print("Pin "); Serial.print(i); Serial.print(" reads: ");
		Serial.println(raw[i]);
	}
	Serial.println();

}

unsigned int * LineSensor::rawArray() {
	unsigned int* raw = new unsigned int[9];

	raw[0] = analogRead(VEXSENSOR);
	raw[1] = analogRead(LINESENSOR1);
	raw[2] = analogRead(LINESENSOR2);
	raw[3] = analogRead(LINESENSOR3);
	raw[4] = analogRead(LINESENSOR4);
	raw[5] = analogRead(LINESENSOR5);
	raw[6] = analogRead(LINESENSOR6);
	raw[7] = analogRead(LINESENSOR7);
	raw[8] = analogRead(LINESENSOR8);

	return raw;
}

int * LineSensor::processedArray() {
	int* processed = new int[9];
	unsigned int* raw = rawArray();

	for (int i = 0; i <= 9; i++) {
		if (raw[i] > calibrationPoint) {
			processed[i] = 1;
		} else {
			processed[i] = 0;
		}
	}

	return processed;
}