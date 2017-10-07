#include "Arduino.h"
#include "Messages.h"
#include <LiquidCrystal.h>

Messages msg;                                                           // The message object to read in commands from reactor control.
unsigned long timeForHeartbeat;                                         // Used for timing the heartbeat.
const int stopornawpin = 8;                                             // This is the stopped or naw digital pin LED.
LiquidCrystal lcd(40, 41, 42, 43, 44, 45);

typedef enum {
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
  nukeInReactor
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
  lcd.begin(16, 2);

  pinMode(stopornawpin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting");
  msg.setup();
  timeForHeartbeat = millis() + 1000;
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
    msg.sendHeartbeat();
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

  // Send radiation level with heartbeat.
}
