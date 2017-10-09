#include "Arduino.h"
#include "Messages.h"
#include <LiquidCrystal.h>
#include <Servo.h>

Messages msg;                                                           // The message object to read in commands from reactor control.
unsigned long timeForHeartbeat;                                         // Used for timing the heartbeat.
const int stopornawpin = 10;                                             // This is the stopped or Auto digital pin LED.

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);

unsigned int radcounter = 0;                                            // This keeps the timing of the radiation message every 2! seconds.
unsigned int robotstatuscounter = 0;                                    // This is for delaying the robot status messages less often than 5 seconds in between each message.
const int testled = 13;                                                 // we have a test led in pin 13.

//Arm Servo variables
Servo jxservo;
Servo gripservo;

int pos = 10;
int posmin = 0;    // variable to store the servo position
int posmax = 500;
const int jxpin = 25; // pwm pin
const int jxmotor = 9; // control pin

const int grippin = 29; // pwm pin 29
const int gripmotor = 8; // control pin

// const int controlmode = 10;

typedef enum {
  Start,
  robotStop,          // This state will show up in every other state as an if() statement. if isStopped() == true, then store prev state and then switch, else doo other state.
  approachNuke,
  pickUpNukeLow,
  goToCenter,
  findUsedDispenser,
  dispenseNukeHigh,
  findNewDispenser,
  pickUpNukeHigh,
  goToReactor,
  dispenseNukeLow,
  nukeInReactor,
  End
} robotdoState;

typedef enum {
  noFuel = 0x00,
  newfuel = 0xFF,
  spentfuel = 0x2C
} radState;

/**
   This "starter" program reads and debug-prints messages from the field. It also sends
   heartbeat messages every second letting the field know that the robot is still running.

   You can use this as the basis for your project, or copy the elements from here into your
   project. If you do that, be sure that you include the Messages and BTComms classes (.cpp
   and .h files) into your new project.
*/

/**
   Initialize the messages class and the debug serial port
*/
void setup() {
  //LCD setup
  lcd.begin(16, 2);
  pinMode(stopornawpin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting");
  msg.setup();
  timeForHeartbeat = millis() + 1000;

  //Servo Setup
  pinMode(jxmotor, INPUT_PULLUP);
  pinMode(gripmotor, INPUT_PULLUP);
  // pinMode(controlmode, INPUT_PULLUP);
  jxservo.attach(jxpin);
  gripservo.attach(grippin);
}

/**
   The loop method runs continuously, so call the Messages read method to check for input
   on the bluetooth serial port and parse any complete messages. Then send the heartbeat if
   it's time to do that.

   For the final project, one good way of implementing it is to use a state machine with high
   level tasks as states. The state will tell what you should be doing each time the loop
   function is called.

   Refer to the state machine lecture or look at the BTComms class for an example on how to
   implement state machines.
*/
void loop() {
  if (msg.readcomms()) { // this line changes
    msg.printMessage();
  }
  if (millis() > timeForHeartbeat) {
    timeForHeartbeat = millis() + 1000;

    // msg.setrobotmovestate(0x01)
    if (robotstatuscounter == 0) // If Now is the time so send the counter
    {
      


      if (radcounter == 0)
      {
        msg.sendHeartbeat();
        msg.setradAlert(0xFF); // new fuel alert = 0xFF, spent fuel = 0x2C
        msg.sendMessage(0x03); // Send radiation
        radcounter = (radcounter + 1) % 3; // this equals either 0, 1, or 2.
      }
      else
      {
        msg.sendHeartbeat();
        radcounter = (radcounter + 1) % 3; // this equals either 0, 1, or 2.
      }
      robotstatuscounter = (robotstatuscounter + 1) % 6;
    }
    else // no robot status
    {




      if (radcounter == 0)
      {
        msg.sendHeartbeat();
        msg.setradAlert(0xFF); // new fuel alert = 0xFF, spent fuel = 0x2C
        msg.sendMessage(0x03); // Send radiation
        radcounter = (radcounter + 1) % 3; // this equals either 0, 1, or 2.
      }
      else
      {
        msg.sendHeartbeat();
        radcounter = (radcounter + 1) % 3; // this equals either 0, 1, or 2.
      }
      robotstatuscounter = (robotstatuscounter + 1) % 6;
    }
  }



  lcd.setCursor(0, 0);
  lcd.print("Supply");
  lcd.setCursor(7, 0);
  lcd.print(msg.readsupply(), HEX);
  lcd.setCursor(0, 1);
  lcd.print("Stored");
  lcd.setCursor(7, 1);
  lcd.print(msg.readstorage(), HEX);

  if (msg.isStopped())
  {
    //Serial.println("Stopped");
    digitalWrite(stopornawpin, LOW); // Inverted logic

  } else {
    //Serial.println("Autonomous");
    digitalWrite(stopornawpin, HIGH); // Inverted logic
  }
}


/*
  // oirnfdgouierfoiuerf
  void startStateRad(bool radornaw, unsigned char radlev) // if no rad, set radornaw to false.
  {
  if (!radornaw)
  {
    if (msg.readcomms()) { // this line changes
      msg.printMessage();
    }
    if (millis() > timeForHeartbeat) {
      timeForHeartbeat = millis() + 1000;
      if (radcounter == 0)
      {
        msg.sendHeartbeat();
        // msg.setradAlert(0x00); // new fuel alert = 0xFF, spent fuel = 0x2C
        // msg.sendMessage(0x03);
        radcounter = (radcounter + 1) % 3; // this equals either 0 or 1.
      }
      else
      {
        msg.sendHeartbeat();
        radcounter = (radcounter + 1) % 3; // this equals either 0 or 1.
      }
    }
  }
  else
  {
    if (msg.readcomms()) { // this line changes
      msg.printMessage();
    }
    if (millis() > timeForHeartbeat) {
      timeForHeartbeat = millis() + 1000;
      if (radcounter == 0)
      {
        msg.sendHeartbeat();
        msg.setradAlert(radlev); // new fuel alert = 0xFF, spent fuel = 0x2C
        msg.sendMessage(0x03);
        radcounter = (radcounter + 1) % 3; // this equals either 0 or 1.
      }
      else
      {
        msg.sendHeartbeat();
        radcounter = (radcounter + 1) % 3; // this equals either 0 or 1.
      }
    }
  }

  }



  /*
  if (msg.readcomms()) { // this line changes
  msg.printMessage();
  }
  if (millis() > timeForHeartbeat) {
  timeForHeartbeat = millis() + 1000;
  if (radcounter == 0)
  {
    msg.sendHeartbeat();
    msg.setradAlert(0x2C); // new fuel alert = 0xFF, spent fuel = 0x2C
    msg.sendMessage(0x03);
    radcounter = (radcounter + 1) % 3; // this equals either 0 or 1.
  }
  else
  {
    msg.sendHeartbeat();
    radcounter = (radcounter + 1) % 3; // this equals either 0 or 1.
  }
  }
  if (msg.isStopped())
  {
  //Serial.println("Stopped");
  lcd.setCursor(0, 1);
  lcd.print("Stopped     ");
  digitalWrite(stopornawpin, LOW); // Inverted logic

  } else {
  //Serial.println("Autonomous");
  lcd.setCursor(0, 1);
  lcd.print("Autonomous");
  digitalWrite(stopornawpin, HIGH); // Inverted logic
  }

*/
