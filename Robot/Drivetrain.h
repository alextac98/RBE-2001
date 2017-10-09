#ifndef DRIVETRAIN_H
#define  DRIVETRAIN_H

#include <Arduino.h>
#include "Motor.h"


class Drivetrain {

	//////////////////////////////////////////////////////////

public:
	//-----Basic Drive Methods--------------------------------  
	Drivetrain();
	void DrivetrainSetup();
	void setPower(int, int); //left, right from -100 to 100  
	
private:
	//-----Drive Motors---------------------------------------
	Motor leftDrive;
	Motor rightDrive;

	//////////////////////////////////////////////////////////

};

#endif