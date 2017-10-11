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
	
	for (int i = 0; i <= 10; i++) {
		Serial.print("Pin "); Serial.print(i); Serial.print(" reads: ");
		Serial.println(raw[i]);
	}
	Serial.println();

}

unsigned int * LineSensor::rawArray() {

	raw[0] = analogRead(VEXSENSOR);
	raw[1] = analogRead(LINESENSOR1);
	raw[2] = analogRead(LINESENSOR2);
	raw[3] = analogRead(LINESENSOR3);
	raw[4] = analogRead(LINESENSOR4);
	raw[5] = analogRead(LINESENSOR5);
	raw[6] = analogRead(LINESENSOR6);
	raw[7] = analogRead(LINESENSOR7);
	raw[8] = analogRead(LINESENSOR8);
	raw[9] = analogRead(VEXSENSORSIDE);

	return raw;
}

int * LineSensor::processedArray() {
	int* processed = new int[10];
	rawArray();

	for (int i = 0; i <= 9; i++) {
		if (raw[i] > calibrationPoint) {
			processed[i] = 1;
		} else {
			processed[i] = 0;
		}
	}

	return processed;
}

float LineSensor::avgLinePos()
{
	// readingSum is just a sum, while positionSum is weighted by position
	rawArray();

	float readingSum = 0.0;
	float positionSum = 0.0;
	for (int i = 1; i <= 8; i++) {
		readingSum += raw[i];
		// Subtract 1 from NUM_LINE_SENSORS to account for the fencepost problem
		// Multiply by 2 and subtract 1 to map the 0-1 range to the -1-1 range
		positionSum += (float)raw[i] * (float)(i);
	}
	return scaleNumber(positionSum/readingSum, 1, 9, -1, 1);
	//return positionSum / readingSum;
}

float LineSensor::scaleNumber(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool LineSensor::isAllBlack() {
	int* values = processedArray();
	for (int i = 1; i <= 9; i++) {
		if (values[i] != 1) {
			return false;
		}
	}
	return true;
}

bool LineSensor::isCentered() {
	int* values = processedArray();
	if (values[4] == 1 && values[5] == 1) {
		if (values[1] == 0 && values[2] == 0 && values[3] == 0 &&
			values[6] == 0 && values[7] == 0 && values[8] == 0) {
			return true;
		}
	}
	return false;
}