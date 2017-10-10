/*
   Messages.h

    Created on: Sep 15, 2016, modded laters
        Author: bradmiller, modded by Andrew Schueelr
*/

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "States.h"

/**
   Handles Bluetooth messages as they are received from the Reactor Control System
   This class keeps the current state of the field and allows your program to query it at
   any time. The read() method is called inside the loop() of your program. It reads a
   message and sets the internal class state based on it's contents. This happens each loop.
   You then write methods (like isStopped(), below) that report on that state. The idea is
   that the receipt and parsing of the messages is decoupled from your use of the state data
   making your programs much less complex.

   You need to add code and private state variables (like stopped, below) to parse and remember
   the RCS state. Then add methods that can be called independent of message receipt to retrieve
   that state.

   Other things you may want to do:
   - check is messages are addressed to your robot (or broadcast to all)
   - handle the source and destination fields which exist for all messages
   - add methods to determine which storage is free/used so you can ask by number
     rather than having to do the bit mask stuff
*/
class Messages {
  public:
    
    Messages();

    bool isStopped(); // Why is this always bolded???
    void sendHeartbeat(); // To be depreciated
    bool readcomms();
    void setup();
    void printMessage();
    
    // Andrew Added
    
    void sendMessage(MessageType msgtype);
    void setradAlert(unsigned char alert);
    
    void setrobotmovestate(unsigned char mover);
    void setrobotgripstate(unsigned char gripper);
    void setrobotopstate(unsigned char thatop);
    
    unsigned char readstorage();
    unsigned char readsupply();
    unsigned char readradAlert();
    
  private:
    // Andrew Added
    unsigned long timecurrent;
    unsigned long timeprev;

    unsigned char availstorage;		// Storage tubes are bits 0-3
    unsigned char availsupply;		// Bits 0-3
    unsigned char radiationalert;

    unsigned char readrobotmove();
    unsigned char prevreadrobotmove();
    unsigned char readrobotgrip();
    unsigned char readrobotopstatus();

    unsigned char robotmove;       // 0x01 = stopped 0x02 = teleop moving, 0x03 auto moving
    unsigned char prevrobotmove;   // Stores the previous movement state: stopped, teleop, or auto.
    unsigned char robotgrip;       // 0x01 = no rod, 0x02 = has rod
    unsigned char robotopstatus;   // What stage is the robot in

};

#endif /* MESSAGES_H_ */
