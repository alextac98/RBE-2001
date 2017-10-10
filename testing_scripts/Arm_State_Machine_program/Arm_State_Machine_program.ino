#include <TimerOne.h>
#include <Servo.h>
/*
  State machine that controls the arm and gripper. Needs to be tested!!!!!!!
        -- Written by Andrew Schueler
        --edited by Josue Contreras
*/
Servo jxservo;
Servo gripservo;

const int jxpin = 28; // pwm pin original 25
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
} ARM;

bool bumper = false;

void setup() {
  Serial.begin(115200);
  jxservo.attach(jxpin);
  gripservo.attach(grippin);
}

void loop()
{
  potVal = analogRead(potpin);
  bumper = armgripper(upperpos1, potVal, 10, bumper);    // We want to return true, which means that the whole move was completed.
  Serial.print(potVal); Serial.print("   "); Serial.println(bumper);
}

// Values for pot are: 937 upper to 622 mid to 5 lower
// 1 means gripper open, 0 means gripper closed
bool armgripper(ARM agposition, int potValue, int threshold, bool bumperstop)
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
        if (!((potValue <= 622 + threshold) && (potValue >= 622 - threshold)))
        {
          jxservo.write(114);
          return false;
        }
        else
        {
          gripservo.write(100); // maybe 180?? open
          return true;
        }
      case lowpos1:           //Arm down, gripper open
        if (!((potValue <= (5 + threshold)) && (potValue >= (5 - threshold))))
        {
          jxservo.write(0);
          return false;
        }
        else
        {
          gripservo.write(100); // maybe 180?? open
          return true;
        }
      case upperpos0:       //Arm up, gripper closed
        if (potValue <= 915 + threshold && potValue >= 915 - threshold) {
          jxservo.write(180);
          return false;
        }
        else
        {
          gripservo.write(0); // maybe 180?? open
          return true;
        }
      case midpos0:         //Arm mid-position, gripper closed
        if (!((potValue <= (622 + threshold) && potValue >= (622 - threshold))))
        {
          jxservo.write(114);
          return false;
        }
        else
        {
          gripservo.write(0); // maybe 180?? open
          return true;
        }
      case lowpos0:         //Arm down, gripper closed
        if (!((potValue <= (259 + threshold) && potValue >= (259 - threshold))))
        {
          jxservo.write(0);
          return false;
        }
        else
        {
          gripservo.write(0); // maybe 180?? open
          return true;
        }
      default:
        Serial.println("Invalid state");
        break;
    }
  }
}

int potconvert(unsigned int valueconvert) {
  return map(valueconvert, 0, 1023, 0, 180);
}
