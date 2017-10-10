#ifndef STATES_H
#define STATES_H

// This header file is fore our enumerations and other states.

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



#endif
