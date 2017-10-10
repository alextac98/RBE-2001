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
const int jxpin = 28; // pwm pin original 25
const int jxmotor = 9; // control pin

const int grippin = 29; // pwm pin 29
const int potpin = A11;       // This is the pin the pot is wired to.

int potVal = 0;               // This is the integer output of the potentiometer.

typedef enum {
  lowpos1,
  lowpos0,
  upperpos1,
  upperpos0,
  midpos1,
  midpos0,
} arm;

bool bumper = false;

void setup() {
  Serial.begin(115200);
  jxservo.attach(jxpin);
  gripservo.attach(grippin);
}

void loop()
{
  potVal = analogRead(potpin);
  bumper = armgripper(upperpos1, potVal, 100, bumper);    // We want to return true, which means that the whole move was completed.
  Serial.print(potVal); Serial.print("   "); Serial.println(bumper);
}

// Values for pot are: 937 upper to 622 mid to 5 lower
// 1 means gripper open, 0 means gripper closed
bool armgripper(arm agposition, int potValue, int threshold, bool bumperstop)
{
  if (!bumperstop)
  {
    switch (agposition)
    {
      case upperpos1://Arm up, gripper open
        if (!((potValue <= (915 + threshold)) && (potValue >= (915 - threshold))))
        { // This is the code that runs
//          agposition = upperpos1;
          jxservo.write(179); // Why is this not happening???
          return false;
        }
        else
        { // This is the code that runs if the arm isn't quite at the correct angle.
          gripservo.write(0); // maybe 180?? open
          return true;
        }
      case midpos1://Arm mid-position, gripper open
        jxservo.write(114);
        if ((potValue <= 622 + threshold) && (potValue >= 622 - threshold))
        {
          gripservo.write(100); // maybe 180?? open
          break;
        }
        break;
      case lowpos1:           //Arm down, gripper open
        jxservo.write(0);
        if (potValue <= 5 + threshold && potValue >= 5 - threshold) {
          gripservo.write(100); // maybe 180?? open
          break;
        }
        break;
      case upperpos0:       //Arm up, gripper closed
        jxservo.write(180);
        if (potValue <= 915 + threshold && potValue >= 915 - threshold) {
          gripservo.write(0); // maybe 180?? open
          break;
        }
        break;
      case midpos0:         //Arm mid-position, gripper closed
        jxservo.write(114);
        if (potValue <= 622 + threshold && potValue >= 622 - threshold) {
          gripservo.write(0); // maybe 180?? open
          break;
        }
        break;
      case lowpos0:         //Arm down, gripper closed
        jxservo.write(0);
        if (potValue <= 259 + threshold && potValue >= 259 - threshold)
        {
          gripservo.write(0); // maybe 180?? open
          break;
        }
        break;
      default:
        Serial.println("Invalid state");
        break;
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


int potconvert(unsigned int valueconvert) {
  return map(valueconvert, 0, 1023, 0, 180);
}
