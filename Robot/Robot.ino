
#include "Motor.h"
#include "Pinout.h"
#include "Drivetrain.h"

Drivetrain drive;

void setup() {
  // put your setup code here, to run once:
	drive.DrivetrainSetup();
}

void loop() {
  // put your main code here, to run repeatedly
	drive.setPower(100, 100);

}
