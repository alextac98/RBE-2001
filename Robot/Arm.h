// Arm.h

#ifndef _ARM_h
#define _ARM_h
#include "Servo.h"
#include "States.h"

typedef enum {
	highpos,
	midpos,
	lowpos
} setArm;

class Arm {

	//////////////////////////////////////////////////////////

public:
	//-----Basic Methods--------------------------------------  
	Arm();

	bool setArmPosition(setArm);
	void openGripper();
	void closeGripper();

	bool command(armPos);

	bool armgripper(armPos, int, int, bool);



private:
	//-----Servos---------------------------------------------
	
	

	//-----Constants------------------------------------------

	const int threshold = 10;
	
	const int closedGripperPos = 0;
	const int openGripperPos = 100;

	const int armLowPOT = 259;
	const int armMidPOT = 622;
	const int armHighPOT = 915;

	const int armLow = 0;
	const int armMid = 114;
	const int armHigh = 180;
	
	//////////////////////////////////////////////////////////

};
#endif

