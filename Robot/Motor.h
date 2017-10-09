// Motor.h

#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "Servo.h"


class Motor {
public:
	Motor(int pinIn, boolean reverse = false);
	void motorSetup();
	void setPower(int);

private:
	long lastSetTime;
	boolean isReverse;
	int pin;
	Servo motor;
};
#endif

