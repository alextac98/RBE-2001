// 
// 
// 

#include "Arm.h"
#include "Pinout.h"

Arm::Arm() {

}

void Arm::armSetup() {
	jxServo.attach(JXSERVO);
	gripper.attach(GRIPSERVO);
}

void Arm::setArmPosition(armPos position) {
	switch (position)
	{
	case up:
		jxServo.write(armUp);
		break;
	case mid:
		jxServo.write(armMid);
		break;
	case down:
		jxServo.write(armDown);
		break;
	default:
		break;
	}
}

bool Arm::isArmPosition(armPos position) {
	int potValue = getArmPosition();
	switch (position)
	{
	case up:
		if (potValue <= isUp + threshold && potValue >= isUp - threshold) {
			return true;
		}
		break;
	case mid:
		if (potValue <= isMid + threshold && potValue >= isMid - threshold) {
			return true;
		}
		break;
	case down:
		if (potValue <= isDown + threshold && potValue >= isDown - threshold) {
			return true;
		}
		break;
	default:
		return false;
		break;
	}
	return false;
}

bool Arm::openGripper() {
	gripper.write(gripperOpen);
	if (lastOpenTime == 0) { lastOpenTime = millis();}

	if (millis() - lastOpenTime > gripperDelay) {
		lastOpenTime = 0;
		return true;
	}
	return false;
}

bool Arm::closeGripper() {
	gripper.write(gripperClosed);
	if (lastClosedTime == 0) { lastClosedTime = millis(); }
	Serial.println(millis() - lastClosedTime);
	if (millis() - lastClosedTime > gripperDelay) {
		lastClosedTime = 0;
		return true;
	}
	return false;
}

int Arm::getArmPosition() {
	return analogRead(POT);
}

