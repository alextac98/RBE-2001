/*
   ArdPID.h

    Created on: 9/29/2017
			by Andrew Schueler
*/

#ifndef ARDPID_H_
#define ARDPID_H_

/**

*/
class ArdPID {
  public:

    ArdPID(unsigned long currtime, double deadBand);							// Constructor
    double pidControl(double setpoint, double currentpoint, double kp, double ki, double kd, double upperrange, double lowerrange,  unsigned long currTime);				// Actual PID method.

  private:
    double pidDeadband(double inval, double deadband);				// If the throttle is above deadband, allow the throttle.
    double inRange(double inval, double upperrange, double lowerrange);					// Is the value in the range of the PID? if not, constrain to max or minimum.
    // Class fields
    // persistent variables for PID control
    double deadband;
    double error;
    double integralerror;
    double deriverror;
    double preverror;
    double currtime; // This is the time elapsed before initializing the PID block.
    unsigned long lastTime;
    double timeInterval; // This is the difference between now and before, and then converted to milliseconds.
    double anoutput;
};

#endif /* ARDPID_H_ */

