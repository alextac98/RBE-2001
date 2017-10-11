#ifndef STATES_H
#define STATES_H

// This header file is fore our enumerations and other states.

typedef enum {
  kReserved,
  kStorageAvailability = 0x01,
  kSupplyAvailability = 0x02, //
  kRadiationAlert = 0x03,
  kStopMovement = 0x04,
  kResumeMovement = 0x05,
  kRobotStatus = 0x06,
  kHeartbeat = 0x07
} MessageType;

typedef enum
{
  Start = 1,
  approachNuke = 2,
  pickUpNukeLow = 3,
  goToCenter = 4,
  findUsedDispenser = 5,
  dispenseNukeHigh = 6,
  findNewDispenser = 7,
  pickUpNukeHigh = 8,
  goToReactor = 9,
  dispenseNukeLow = 10,
  nukeInReactor = 11,
  End = 12,
  robotStop = 13  // This state will show up in every other state as an if() statement. if isStopped() == true, then store prev state and then switch, else doo other state.
} robotdoState;

typedef enum
{
  noFuel = 0x00,
  newfuel = 0xFF,
  spentfuel = 0x2C
} radState;

typedef enum
{
  stopped = 0x01,
  teleop = 0x02,
  autonomous = 0x03
} robotMovingState;

typedef enum
{
  norod = 0x01,
  hasrod = 0x02
} robotGripState;

typedef enum
{
  gripattempt = 0x01,
  griprelease = 0x02,
  drivetoreactorrod = 0x03,
  drivetostoragearea = 0x04,
  drivetosupplyarea = 0x05,
  idle = 0x06 // no op in progress
} robotOpState;

typedef enum {
  lowpos1,
  lowpos0,
  upperpos1,
  upperpos0,
  midpos1,
  midpos0,
} ARM;

#endif
