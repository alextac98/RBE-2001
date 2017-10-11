#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

//-----Drive Train Constants---------------------------
#define DRIVELEFT 11
const bool isDriveLeftReversed = false;
//#define ENCODERLEFT1 2
//#define ENCODERLEFT2 22

#define DRIVERIGHT 4
const bool isDriveRightReversed = true;
//#define ENCODERRIGHT1 3
//#define ENCODERRIGHT2 23

//-----FourBar Constants--------------------------------
#define FOURBARMOTOR 28
#define GRIPPERMOTOR 29

#define TUBESENSOR 23

#define POT 11            // A11

//-----Line Following Constants-------------------------
#define VEXSENSOR 0

#define LINESENSOR1 1
#define LINESENSOR2 2
#define LINESENSOR3 3
#define LINESENSOR4 4
#define LINESENSOR5 5
#define LINESENSOR6 6
#define LINESENSOR7 7
#define LINESENSOR8 8

//-----Bluetooth Constants------------------------------

#define DRIVEMODELED 10

//-----Display Constants--------------------------------
#define DISPLAY1 40
#define DISPLAY2 41
#define DISPLAY3 42
#define DISPLAY4 43
#define DISPLAY5 44
#define DISPLAY6 45

#define STATUSLIGHT 10

//-----Servo Constants----------------------------------

#define JXSERVO 28
#define GRIPSERVO 29

//------------------------------------------------------
#endif
