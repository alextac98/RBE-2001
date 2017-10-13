/*
   Messages.cpp

    Created on: Sep 15, 2016, modded later
        Author: bradmiller, modded by Andrew Schueler
*/

#include "Arduino.h"
#include "Messages.h"
#include "BTComms.h"
#include "States.h"

BTComms comms;

/**    Constuctor
   Initialize everything here when the class is created
   Note: you cannot call methods that depend on other classes having already been created
*/

Messages::Messages() {
  timecurrent = millis();                 // This is for the heartbeat and ratiation alert.
  timeprev = 0;                           // These 2 aren't implemented yet.

  availstorage = 0x00;                    // Initialize to 0.
  availsupply = 0x00;
  radiationalert = 0x00;                  // We don't start with radiation. 0x2C spent fuel, 0xFF is new fuel

  storerod = 1;								// We initialize the storerod 



  robotmove = 0x01;       // 0x01 = stopped 0x02 = teleop moving, 0x03 auto moving
  prevrobotmove = 0x03;   // This is how we store the previous movement variable.
  robotgrip = 0x01;       // 0x01 = no rod, 0x02 = has rod
  robotopstatus = 0x06;   // 0x01 = grip attempt in process, 0x02 grip release in progress,
  // 0x03 = driving to reactor rod, 0x04 = driving to storage area, 0x05 = Driving to supply area, 0x06 = Idle, no opp in progress
}

int Messages::whichStore()
{
	int i = 1;
	unsigned char startrods = (availstorage << 4) | 0x0F; // 0xFz
	// assume availstorage '1' means full
	for (i = 4; i >= 1; i--)

	{
		if ((startrods | 0x7F) == 0x7F)
		{
			return i;
		}
		else
		{
			//i++;
			startrods = ((startrods << 1) | 0x0F);
		}
		// return 0;
	}
}

int Messages::whichSupply()                      // This is like the previous two methods above, but for supply tubes.
{
  int c = 1;
  unsigned char suprods = availsupply;
  // assume availstorage '1' means full
  for (c = 4; c <= 1; c--){
    if ((suprods | 0xFE) == 0xFE)
    {
      return c;
    }
    else
    {
      suprods = ((suprods >> 1) | 0xF0);
    }
    // return 0;
  }
}

int getwhichSupply(); 


/*  Returns if the state is 0x00 "Reserved" or 0x01 "Stopped".*/
bool Messages::isStopped() {
  return robotmove == 0x01 || robotmove == 0x00;
}

/*  Set radiation alert.*/
void Messages::setradAlert(unsigned char alert) {
  radiationalert = alert;
}

/*  Set robotmove status*/
void Messages::setrobotmovestate(unsigned char mover) {
  robotmove = mover;
}

/*  Set robotgrip status*/
void Messages::setrobotgripstate(unsigned char gripper) {
  robotgrip = gripper;
}

/*  Set robotopstatus status*/
void Messages::setrobotopstate(unsigned char thatop) {
  robotopstatus = thatop;
}

// Here are the getters

/*  Get radiation alert.*/
unsigned char Messages::readradAlert() {
  return radiationalert;
}

/*  Get storage bins*/
unsigned char Messages::readstorage() {
  return availstorage;
}

/*  Get supply bins.*/
unsigned char Messages::readsupply() {
  return availsupply;
}

/*  Get robotmove state*/
unsigned char Messages::readrobotmove() {
  return robotmove;
}

/*  Get prevrobotmove state*/
unsigned char Messages::prevreadrobotmove() {
  return prevrobotmove;
}

/*  Get robotgrip state*/
unsigned char Messages::readrobotgrip() {
  return robotgrip;
}

/*  Get robotopstatus state*/
unsigned char Messages::readrobotopstatus() {
  return robotopstatus;
}

/* This is the getter for which storage tube to use.*/
int Messages::getwhichstore()
{
	return (int)storerod;
}

/**   Setup class code that is called from the Arduino sketch setup() function. This doesn't
   get called until all the other classes have been created.
*/
void Messages::setup() {
  comms.setup();
}

/**
   Send a heartbeat message to the field to let it know that your code is alive
   This should be called by your robot program periodically, say once per second. This
   timing can easily be done in the loop() function of your program.
*/
// This will become depreciated when I create a sendMessage method.
void Messages::sendHeartbeat() {
  comms.writeMessage(kHeartbeat, 0x0A, 0x00);
}

/**
   Print message for debugging
   This method prints the message as a string of hex numbers strictly for debugging
   purposes and is not required to be called for any other purpose.
*/
void Messages::printMessage() {
  for (int i = 0; i < comms.getMessageLength(); i++) {
    Serial.print(comms.getMessageByte(i), HEX);
    Serial.print(" ");
  }
  Serial.println();
}

/**
   Read messages from the Bluetooth serial connection
   This method should be called from the loop() function in your arduino code. It will check
   to see if the lower level comms object has received a complete message, and run the appropriate
   code to handle the message type. This should just save the state of the message inside this class
   inside member variables. Then add getters/setters to retrieve the status from your program.
*/
bool Messages::readcomms()
{
  if (comms.readSwag())
  {
    if (comms.getMessageByte(1) == 0x0A || comms.getMessageByte(1) == 0x00) // Because we are team 10!!!!, do we want (1) or (2) for getMessageByte()???
    {
      switch (comms.getMessageByte(0)) {
        case kStorageAvailability:
          // Availability mask length of '1'   Bitmask of available tubes. Bit 0 = tube 1; Bit 1 = tube 2, ...  If tube bit = 0, tube is empty; if tube bit = 1, tube is occupied
          // Supply does not have the cone, storage has the cone.
          availstorage = comms.getMessageByte(3);           // STORAGE tubes are wanted to be bits 0-3
		  storerod = whichStore();
		  break;
        case kSupplyAvailability:
          availsupply = comms.getMessageByte(3);    // SUPPLY tubes are wanted to be bits 0-3
          break;
        case kRadiationAlert:
          // We don't receive the alert. We send it.
          break;
        case kStopMovement:
          // prevrobotmove = readrobotmove();
          robotmove = 0x01; // stipped is true
          break;
        case kResumeMovement:
          robotmove = 0x03; //prevrobotmove;
          break;
        case kRobotStatus:
          // we don't receive robot status, do we?
          break;
        case kHeartbeat:
          // We don't receive heatbeats, we send heartbeats.
          break;
      }
      return true;
    }
  }
  return false;
}

// This method figures out what data to send given the type of message you input. There are only 3 types of messages to send.
void Messages::sendMessage(MessageType msgtype) {
  switch (msgtype)  {
    case kStorageAvailability:
      // Availability mask length of '1'       Bitmask of available tubes. Bit 0 = tube 1; Bit 1 = tube 2, ...  If tube bit = 0, tube is empty; if tube bit = 1, tube is occupied
      // Supply does not have the cone, storage has the cone.
      // We don't send this.
      break;
    case kSupplyAvailability:
      // We don't send this.
      break;
    case kRadiationAlert:
      // No more than once per second!!!!
      // If there is any radiation, then output the message. Else, we don't send.
      if (radiationalert != 0x00)
      {
        comms.writeMessageRad(0x0A, 0x00, radiationalert);
      }      
      break;
    case kStopMovement:
      // Don't send anythign here.
      break;
    case kResumeMovement:
      // We don't send this.
      break;
    case kRobotStatus:
      comms.writeMessageRobotStatus(0x0A, 0x00, readrobotmove(), readrobotgrip(), readrobotopstatus());
      break;
    case kHeartbeat:
      comms.writeMessage(kHeartbeat, 0x0a, 0x00);
      break;
  }
}
