// Arm.h

#ifndef _ARM_h
#define _ARM_h
#include "Servo.h"

typedef enum {
	down,
	mid,
	up
} armPos;

class Arm {
public:
	Arm();
	void armSetup();
	void setArmPosition(armPos);
	bool isArmPosition(armPos);

	bool closeGripper();
	bool openGripper();

	int getArmPosition();
private:
	Servo jxServo;
	Servo gripper;

	long lastClosedTime;
	long lastOpenTime;
	const int gripperDelay = 500; //in millis

	const int gripperClosed = 100;
	const int gripperOpen = 0;

	const int armUp = 176;
	const int armMid = 114;
	const int armDown = 0;

	const int threshold = 10;
	const int isUp = 915;
	const int isMid = 728;
	const int isDown = 262;
};


#endif

