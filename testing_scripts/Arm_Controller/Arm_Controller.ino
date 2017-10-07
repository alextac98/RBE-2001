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
  pinMode(jxmotor, INPUT_PULLUP);
  pinMode(gripmotor, INPUT_PULLUP);
  pinMode(controlmode, INPUT_PULLUP);
  jxservo.attach(jxpin);
  gripservo.attach(grippin);
}

void loop() {
  if (digitalRead(controlmode))
  {
    // This is for the reactors only, where the full range of motion is used.
    if (!digitalRead(jxmotor)) {
      jxservo.write(180);
    }
    else
    {
      jxservo.write(0);
    }
    if (!digitalRead(gripmotor)) {
      gripservo.write(100); // maybe 180??
    }
    else
    {
      gripservo.write(0);
    }
  }
  else
  {
    // This is for the supply and storage only, where the full range of motion is used.
    if (!digitalRead(jxmotor)) {
      jxservo.write(180);
    }
    else
    {
      jxservo.write(114);
    }
    if (!digitalRead(gripmotor)) {
      gripservo.write(100); // maybe 180??
    }
    else
    {
      gripservo.write(0);
    }
  }
}

