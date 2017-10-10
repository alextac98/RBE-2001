// 
// 
// 

#include "Arm.h"
#include "States.h"
#include "Pinout.h"

Servo jxservo;
Servo gripservo;

Arm::Arm
() {
	jxservo.attach(FOURBARMOTOR);
	gripservo.attach(GRIPPERMOTOR);
	//jxservo.write(180);
	//gripservo.write(180);
	//Serial.println("delaying");
	//delay(10000);
}

bool Arm::armgripper(armPos agposition, int potValue, int threshold, bool bumperstop)
{
	if (!bumperstop)
	{
		switch (agposition)
		{
		case highposOpen://Arm up, gripper open
			if (!((potValue <= (915 + threshold)) && (potValue >= (915 - threshold))))
			{ // This is the code that runs

				jxservo.write(179); // Why is this not happening???
				return false;
			}
			else
			{ // This is the code that runs if the arm isn't quite at the correct angle.
				gripservo.write(0); // maybe 180?? open
				return true;
			}
		case midposOpen://Arm mid-position, gripper open
			if (!((potValue <= 622 + threshold) && (potValue >= 622 - threshold)))
			{
				jxservo.write(114);
				return false;
			}
			else
			{
				gripservo.write(100);
				return true;
			}
		case lowposOpen:           //Arm down, gripper open
			if (!((potValue <= (5 + threshold)) && (potValue >= (5 - threshold))))
			{
				jxservo.write(0);
				return false;
			}
			else
			{
				gripservo.write(100);
				return true;
			}
		case highposClose:       //Arm up, gripper closed
			if (potValue <= 915 + threshold && potValue >= 915 - threshold) {
				jxservo.write(180);
				return false;
			}
			else
			{
				gripservo.write(0);
				return true;
			}
		case midposClose:         //Arm mid-position, gripper closed
			if (!((potValue <= (622 + threshold) && potValue >= (622 - threshold))))
			{
				jxservo.write(114);
				return false;
			}
			else
			{
				gripservo.write(0); // maybe 180?? open
				return true;
			}
		case lowposClose:         //Arm down, gripper closed
			if (!((potValue <= (259 + threshold) && potValue >= (259 - threshold))))
			{
				jxservo.write(0);
				return false;
			}
			else
			{
				gripservo.write(0);
				return true;
			}
		default:
			Serial.println("Invalid state");
			break;
		}
	}
}

//bool Arm::command(armPos command) {
//	switch (command)
//	{
//	case upperposOpen://Arm up, gripper open
//		if (setArmPosition(highpos)) {
//			openGripper();
//			return true;
//		}
//		return false;
//	case midposOpen://Arm mid-position, gripper open
//		if (setArmPosition(midpos)) {
//			openGripper();
//			return true;
//		}
//		return false;
//	case lowposOpen:           //Arm down, gripper open
//		if (setArmPosition(lowpos)) {
//			openGripper();
//			return true;
//		}
//		return false;
//	case upperposClose:       //Arm up, gripper closed
//		if (setArmPosition(highpos)) {
//			closeGripper();
//			return true;
//		}
//		return false;
//	case midposClose:         //Arm mid-position, gripper closed
//		if (setArmPosition(midpos)) {
//			closeGripper();
//			return true;
//		}
//		return false;
//	case lowposClose:         //Arm down, gripper closed
//		if (setArmPosition(lowpos)) {
//			closeGripper();
//			return true;
//		}
//		return false;
//
//	default:
//		Serial.println("Invalid state");
//		break;
//	}
//}
//
//bool Arm::setArmPosition(setArm armPosition) {
//	int potValue = analogRead(POT);
//
//		switch (armPosition) {
//		case highpos:
//			jxservo.write(armHigh);
//			if (potValue <= (622 + threshold) && potValue >= (622 - threshold)) {
//				return true;
//			}
//			else {
//				return false;
//			}
//			break;
//		case midpos:
//			if (!(potValue <= (622 + threshold) && potValue >= (622 - threshold))) {
//				jxservo.write(armMid);
//				return true;
//			}
//			else {
//				return false;
//			}
//			break;
//		case lowpos:
//			jxservo.write(armLow);
//			if (potValue <= (622 + threshold) && potValue >= (622 - threshold)) {
//				return true;
//			}
//			else {
//				return false;
//			}
//			break;
//		default:
//			return false;
//			break;
//		}
//}
//
//void Arm::openGripper() {
//	gripservo.write(openGripperPos);
//}
//
//void Arm::closeGripper() {
//	gripservo.write(0);
//	Serial.println("hi");
//}