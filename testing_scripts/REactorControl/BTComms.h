#ifndef _BTReader
#define _BTReader

/**
   Low level class to both receive and send message to the field.
   The writeMessage() method sends messages to the field and the other methods
   receive message from the field.

   This class is used by the higher level Messages class to separate the actual
   byte reading and dealing with checksums from the messages class to make it more
   understandable.
*/

class BTComms {
  public:
    BTComms();
    void setup(); // Initialize serial
    int getMessageLength(); // Get the the first element in the message. It denotes message length in bytes.
    unsigned char getMessageByte(unsigned index); // Gets that element in the array.
    bool readSwag(); // After printing the message to the message object, return a bool saying that the message has updated.
    void writeMessage(unsigned char b1, unsigned char b2, unsigned char b3); // For heartbeat only
    void writeMessageRad(unsigned char source, unsigned char dest, unsigned char radlevel); // For radiation only
    // For Robot status only. Requires only 3 bytes.
    void writeMessageRobotStatus(unsigned char source, unsigned char dest, unsigned char status1, unsigned char status2, unsigned char status3);

  private:

    enum BTstate {kLookingForStart, kReadingMessageLength, kReadMessage} BTstate; // The 3 only sending states we need.
    unsigned messageLength; // the message length variable.
    static const int messageBufferLength = 20;
    unsigned char message[messageBufferLength];  // This is super large so that we never have overflow.
    unsigned messageIndex;  // Index of the message array???
    unsigned char kMessageStart = 0x5f; // This is the start of a message
};

#endif
