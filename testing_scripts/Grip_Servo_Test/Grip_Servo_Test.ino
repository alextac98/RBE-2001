#include <Servo.h>

Servo myservo;

int pos = 10;
int posmin = 0;    // variable to store the servo position
int posmax = 500;
const int jxpin = 6;
const int control = 13;
/*typedef enum {
  opengrip,
  closedgrip
} gripstate;*/

bool closed = false;

void setup() {
  pinMode(control, INPUT_PULLUP);
  myservo.attach(jxpin);
}

void loop() {

  if (!digitalRead(control)) {
    myservo.write(100);
  }
  else
  {
    myservo.write(0);
  }
}

