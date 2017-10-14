
#include "Motor.h"

Motor::Motor(int pinIn, boolean reverse) :motor() {
	pin = pinIn;
	isReverse = reverse;
}
/**
* Sets up the motor
*/
void Motor::motorSetup() {
	motor.attach(pin, 1000, 2000);
	setPower(0);
}

/*
* Sets power to the motor. Anything under 20% speed will automatically become 0 for stall prevention
* @param int power from -100 to 100
*/
void Motor::setPower(int power) {     //input from -100 to 100
	if (abs(power) < lowSpeed) {              //used to prevent stall at low set speed
		power = 0;
	}

	if ((millis() - lastSetTime) > 20) {	//prevents from updating the motor too quickly
		if (isReverse == true) {
			power *= -1;
		}
		motor.write(constrain(map(power, -100, 100, 0, 180), 0, 180));
		lastSetTime = millis();
	}
}

