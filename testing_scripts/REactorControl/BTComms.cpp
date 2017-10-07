#include "Arduino.h"
#include "BTComms.h"

/**
   Bluetooth communications constructor
*/

// Somebody else wrote this, but Andrew Schueler is porting this to the BeagleBone Blue.

BTComms::BTComms() {
  messageIndex = 0;
  messageLength = 0;
  BTstate = kLookingForStart;
}

/**
   Code that is called from the arduino setup() function
   This initializes things that cannot be set up from the constructor.
*/
void BTComms::setup() {
  Serial3.begin(115200);		// This is the correct baud.
}

/**
   Send a message to the RCS that has 3 values (opcode, source, dest)
   This method sends messages via bluetooth to the field that have an opcode with
   a source and destination address. It is used for the heartbeat message that has
   no message data.

   You could add additional methods exactly like this one, that take a payload such
   as a status message. You can create a new method that is exactly the same as this
   one (also called writeMessage), but with an additional parameter that gets sent.
   With C++ you can have multiple methods with the same name that are different by
   the number of parameters they have. Be sure that the new function adjusts the length,
   and writes the extra byte to the bluetooth interface and includes it in the checksum
   calculation.
*/

// This message is for sending heartbeats. For future use though, we could send other states as well.
//                comms.writeMessage(kHeartbeat, 0x0A, 0x00);
void BTComms::writeMessage(unsigned char opcode, unsigned char source, unsigned char dest) { // For heartbeat
  // This will need to change. I should note though that the bluetooth chip communicates over UART3.
  // Funny how things are related.
  Serial3.write(kMessageStart);   // 0x5f
  Serial3.write(5);               // Length
  Serial3.write(opcode);          // This would be heartbeat, so yeah
  Serial3.write(source);          //
  Serial3.write(dest);
  Serial3.write(0xff - (opcode + source + dest + 5));
}

// This is strictly designed to send radiation levels.
void BTComms::writeMessageRad(unsigned char source, unsigned char dest, unsigned char radlevel) {
  // This will need to change. I should note though that the bluetooth chip communicates over UART3.
  // Funny how things are related.
  Serial3.write(kMessageStart);   // 0x5f
  Serial3.write(6);               // Length
  Serial3.write(0x03);            // Radiation level opcode
  Serial3.write(source);          //
  Serial3.write(dest);
  Serial3.write(radlevel);
  Serial3.write(0xff - (0x03 + source + dest + radlevel + 6));
}

// This is strictly for robot code.
void BTComms::writeMessageRobotStatus(unsigned char source, unsigned char dest, unsigned char status1, unsigned char status2, unsigned char status3) {
  // This will need to change. I should note though that the bluetooth chip communicates over UART3.
  // Funny how things are related.
  Serial3.write(kMessageStart);   // 0x5f
  Serial3.write(8);               // Length
  Serial3.write(0x06);            // Robot status opcode
  Serial3.write(source);          //
  Serial3.write(dest);
  Serial3.write(status1);
  Serial3.write(status2);
  Serial3.write(status3);
  Serial3.write(0xff - (0x06 + source + dest + status1 + status2 + status3 + 8));
}

/**
   Get the length of the currently received message
   @returns int The number of bytes in the received message
*/
int BTComms::getMessageLength() {
  return messageLength;
}

/**
   Get a byte from the current message
   Retrieve a byte from the currently received message. Only a single message is
   remembered at any time, so you have to call read(), notice that there is a message,
   and then do something with the message bytes.
   @param index The offset (zero-based) to the byte to be returned
   @returns unsigned char The byte that is at the specified index
*/
unsigned char BTComms::getMessageByte(unsigned index) {
  return message[index];
}

/**
   Read a message from Bluetooth
   This method reads messages from Bluetooth by looking for the message start byte, then
   reading the message length and data.

   You should probably modify this to ignore messages with invalid checksums!
*/

// This is the stable bluetooth read method.
bool BTComms::readSwag() {
  while (Serial3.available()) {
    unsigned inByte = Serial3.read();
    switch (BTstate) {
      case kLookingForStart:
        if (inByte != kMessageStart)
          break;
        BTstate = kReadingMessageLength;
        break;
      case kReadingMessageLength:
        messageLength = inByte - 1;
        if (messageLength >= messageBufferLength) {
          Serial.println("Received message length greater than buffer size");
          BTstate = kLookingForStart;
          break;
        }
        messageIndex = 0;
        BTstate = kReadMessage;
        break;
      case kReadMessage:
        message[messageIndex++] = inByte;
        if (messageIndex >= messageLength) {
          BTstate = kLookingForStart;
          return true;
        }
        break;
      default:
        Serial.println("Invalid state");
    }
  }
  return false;
  }

/*  // This is the broken check checksum code.

bool BTComms::readSwag() {
  while (Serial3.available())
  {
    unsigned inByte = Serial3.read(); // This reads
    switch (BTstate)
    {
      case kLookingForStart:
        if (inByte != kMessageStart)
          break;
        BTstate = kReadingMessageLength;
        break;
      case kReadingMessageLength:
        messageLength = inByte - 1;
        if (messageLength >= messageBufferLength) {
          Serial.println("Received message length greater than buffer size");
          BTstate = kLookingForStart;
          break;
        }
        messageIndex = 0;
        BTstate = kReadMessage;
        break;
      case kReadMessage:
        message[messageIndex++] = inByte;
        if (messageIndex >= messageLength) {
          // Before break, set this.
          BTstate = kLookingForStart;
          // This is where we check for invalid checksum!!!
          unsigned char swag42;
          for (int t = 0; t < (messageLength - 1), t++;) // the -1 means we don't check checksum.
          {
            // Starting from the length byte, add all the bytes untill the last data byte is added.
            swag42 += getMessageByte(t); // Stop before before checksum byte.
            // Notice that there will be lots of overflow.
          }

          swag42 = (0xff - swag42); // Now this is a checksum.

          // If the sum of all those bytes is equal to the check
          if (swag42 == getMessageByte(messageLength)) // (getMessageByte(0) + getMessageByte(1) + get)
          {
            return true;
          }
          return false;
        }
        break;

      default:
        Serial.println("Invalid state");
    }
  }
  return false;
}

*/
