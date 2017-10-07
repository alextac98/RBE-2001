#include "ArdPID.h"

ArdPID::ArdPID(unsigned long currTime, double deadBand) {
  // Error handling
  deadband = deadBand;
  double error = 0;
  double integralerror = 0;
  double deriverror = 0;
  //
  double preverror = 0;
  //
  currtime = currTime; // This is the time elapsed before initializing the PID block.
  unsigned long lastTime = 0;
  double timeInterval = 0; // This is the difference between now and before, and then converted to milliseconds.
  //
  double anoutput = 0;
}

/**
   Setup class code that is called from the Arduino sketch setup() function. This doesn't
   get called until all the other classes have been created.
*/

double ArdPID::pidControl(double setpoint, double currentpoint, double kp, double ki, double kd, double upperrange, double lowerrange,  unsigned long currTime)
{
  currtime = currTime;
  error = setpoint - currentpoint;
  integralerror += error;
  timeInterval = currtime - lastTime;
  deriverror = (error - preverror) / timeInterval;
  anoutput = (kp * error) + (ki * integralerror) + (kd * deriverror);
  preverror = error;
  lastTime = currtime;
  return pidDeadband(inRange(anoutput, upperrange, lowerrange), deadband);
}

double ArdPID::pidDeadband(double inval, double deadband) {
  if (inval < -deadband || inval > deadband)
  {
    return inval;
  } else return 0;
}

double ArdPID::inRange(double inval, double upperrange, double lowerrange)
{
  if (inval < lowerrange)
  {
    return lowerrange;
  } else if (inval > upperrange)
  {
    return upperrange;
  } else return inval;
}
