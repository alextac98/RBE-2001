#include <TimerOne.h>
#include <Servo.h>


/*
  State machine that controls the arm and gripper. Needs to be tested!!!!!!!

        -- Written by Andrew Schueler
        --edited by Josue Contreras
*/

Servo jxservo;
Servo gripservo;

long counter1 = 0;
long counter2 = 0;
long difference = 0;

int pos = 10;
int posmin = 0;    // variable to store the servo position
int posmax = 500;
const int jxpin = 25; // pwm pin original 25
const int jxmotor = 9; // control pin

const int grippin = 29; // pwm pin 29
//const int gripmotor = 8; // control pin

const int controlmode = 10;

int potValue;

typedef enum {
  lowpos1,
  lowpos0,
  upperpos1,
  upperpos0,
  midpos1,
  midpos0,
} arm;

bool bumper = true;


void setup() {
  //pinMode(jxmotor, INPUT_PULLUP); //0V down (0 degrees), 5V up (180 degrees)
  //pinMode(gripmotor, INPUT_PULLUP); //
  pinMode(controlmode, INPUT_PULLUP);
  Serial.begin(115200);
  jxservo.attach(jxpin);
  gripservo.attach(grippin);
}

void loop() {
  
  //armgripper(upperpos0, 50, bumper);
  //armgripper(upperpos1,30, bumper);
  //jxservo.write(180); gripservo.write(1);
  //armgripper(lowpos0, 30, bumper);
  //armgripper(lowpos1, 30, bumper);
  
//  jxservo.write(180);
potValue = analogRead(A11); Serial.println(potValue);
  
  //armgripper(midpos0);
  //armgripper(midpos1);
//  potValue = analogRead(A11);
 Serial.println(potValue);
//  delay(1000);
bumper = false;
}

//Values for pot are: 937 upperto 622 mid to 5 lower 
//1 means gripper open, O menas gripper closed
void armgripper(arm agposition, int threshold, bool bumperstop) {
  if (bumperstop) {

     //Serial.println(potValue);
    
    switch (agposition) {

      case upperpos1://Arm up, gripper open
        jxservo.write(180);
        potValue = analogRead(A11);
        Serial.println(potValue);
        if (potValue <= 915 + threshold && potValue >= 915 - threshold) {
          gripservo.write(0); // maybe 180?? open
          Serial.print(potValue);
          break;
        }

      case midpos1://Arm mid-position, gripper open
        jxservo.write(114);
        potValue = analogRead(A11);
        if (potValue <= 622 + threshold && potValue >= 622 - threshold) {
          gripservo.write(100); // maybe 180?? open
          break;
        }

      case lowpos1:           //Arm down, gripper open
        jxservo.write(0);
        potValue = 0;
        potValue = analogRead(A11);
        if (potValue <= 5 + threshold && potValue >= 5 - threshold) {
          gripservo.write(100); // maybe 180?? open
          break;
        }

      case upperpos0://Arm up, gripper closed
        jxservo.write(180);
        potValue = analogRead(A11);
        if (potValue <= 915 + threshold && potValue >= 915 - threshold) {
          gripservo.write(0); // maybe 180?? open
          break;
        }

      case midpos0://Arm mid-position, gripper closed
        jxservo.write(114);
        potValue = analogRead(A11);
        if (potValue <= 622 + threshold && potValue >= 622 - threshold) {
          gripservo.write(0); // maybe 180?? open
          break;
        }

      case lowpos0://Amr down, gripper closed
        jxservo.write(0);
        potValue = analogRead(A11);
        if (potValue <= 259 + threshold && potValue >= 259 - threshold) {
          gripservo.write(0); // maybe 180?? open
          break;
        }
        

        /*
            case gripopen:
              gripservo.write(100); // maybe 180??
              //agposition = none;
              break;

            case gripclosed:
              gripservo.write(0);
              //agposition = none;
              break;
        */

        //BONUS: LED for
    }
  }
}

int potconvert(){
  int valueconvert;
  valueconvert = map(valueconvert, 0, 1023, 0, 180);
  return valueconvert;
}

