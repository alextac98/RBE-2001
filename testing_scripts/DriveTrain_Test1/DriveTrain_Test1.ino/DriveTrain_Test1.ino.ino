
#include <Servo.h>

Servo leftMotor;
Servo rightMotor; 
int pos =0;

void setup() {
  leftMotor.attach(11);  // attaches the servo on pin 9 to the servo object
  rightMotor.attach(6);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    leftMotor.write(pos); 
    rightMotor.write(pos);// tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    leftMotor.write(pos);
    rightMotor.write(pos);// tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
