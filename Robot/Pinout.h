#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

//-----Drive Train Constants---------------------------
#define DRIVELEFT 11
const bool isDriveLeftReversed = false;
//#define ENCODERLEFT1 2
//#define ENCODERLEFT2 22

#define DRIVERIGHT 6
const bool isDriveRightReversed = true;
//#define ENCODERRIGHT1 3
//#define ENCODERRIGHT2 23

//-----FourBar Constants--------------------------------
#define FOURBARMOTOR 25
#define GRIPPERMOTOR 29

//#define UPSENSOR
//#define MIDSENSOR
//#define LOWSENSOR

#define TUBESENSOR

#define POT 11

//-----Line Following Constants-------------------------
#define LINESENSOR	//SDA-20 SCL-21
#define VEXSENSOR

//-----Bluetooth Constants------------------------------


//-----Display Constants--------------------------------
#define DISPLAY1 40
#define DISPLAY2 41
#define DISPLAY3 42
#define DISPLAY4 43
#define DISPLAY5 44
#define DISPLAY6 45

#define STATUSLIGHT 10

//------------------------------------------------------
#endif