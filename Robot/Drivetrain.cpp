#include "Drivetrain.h"
#include "Pinout.h"

Drivetrain::Drivetrain() :
	leftDrive(DRIVELEFT, isDriveLeftReversed),
	rightDrive(DRIVERIGHT, isDriveRightReversed)
{
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----Drive Methods------------------------------------------------------------------------------------------------------------------
void Drivetrain::DrivetrainSetup() {//handles all setup calls
	leftDrive.motorSetup();
	rightDrive.motorSetup();
	Serial.begin(9600);
}

void Drivetrain::setPower(int left, int right) { //from -100 to 100
	leftDrive.setPower(left);
	rightDrive.setPower(right);
}