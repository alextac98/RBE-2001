#include <Servo.h>

/*
  This program works perfectly fine. Don't change this file!!!!!!!

        -- Written by Andrew Schueler
*/

Servo jxservo;
Servo gripservo;

int pos = 10;
int posmin = 0;    // variable to store the servo position
int posmax = 500;
const int jxpin = 25; // pwm pin
const int jxmotor = 9; // control pin

const int grippin = 29; // pwm pin 29
const int gripmotor = 8; // control pin

const int controlmode = 10;

void setup() {
  pinMode(jxmotor, INPUT_PULLUP); //0V down (0 degrees), 5V up (180 degrees)
  pinMode(gripmotor, INPUT_PULLUP); //
  pinMode(controlmode, INPUT_PULLUP);
  jxservo.attach(jxpin);
  gripservo.attach(grippin);
}

void loop() {
  if (digitalRead(controlmode))//True if unconnected ,full range
  {
    // This is for the reactors only, where the full range of motion is used.
    if (!digitalRead(jxmotor)) {// put arm up completely
      jxservo.write(180);
    }
    else //put arm down completely
    {
      jxservo.write(0);
    }
    if (!digitalRead(gripmotor)) {//opened
      gripservo.write(100); // maybe 180??
    }
    else
    {
      gripservo.write(0);//closed
    }
  }
  //If pin 10 is connected to GND, False and mid range
  else
  {
    // This is for the supply and storage only, where the full range of motion is used.
    if (!digitalRead(jxmotor)) {
      jxservo.write(180); //Arm completly up
    }
    else
    {
      jxservo.write(114);//Arm mid way
    }
    if (!digitalRead(gripmotor)) {//Opened
      gripservo.write(100); // maybe 180??
    }
    else
    {
      gripservo.write(0); //closed
    }
  }
}

void armgripper() {
  switch (agposition) {

    case lowpos1://Arm down, gripper open
      jxservo.write(0);
      gripservo.write(100); // maybe 180??
      break;

    case lowpos0://Amr down, gripper closed
      jxservo.write(0);
      gripservo.write(0);
      break;

    case upperpos1://Arm up, gripper open
      jxservo.write(180);
      gripservo.write(100); // maybe 180??
      break;

    case upperpos0://Arm up, gripper closed
      jxservo.write(180);
      gripservo.write(0);
      break;

    case midpos1://Arm mid-position, gripper open
      jxservo.write(114);
      gripservo.write(100); // maybe 180??
      break;

    case midpos0;//Arm mid-position, gripper closed
      jxservo.write(114);
      gripservo.write(0);
      break;
  }
}

