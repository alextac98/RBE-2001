// LineSensor.h
// Code forked from https://github.com/RattyDAVE/ArduinoRobot/tree/master/libraries/SunFounderLineFollower

#ifndef _LINEFOLLOWER_h
#define _LINEFOLLOWER_h

class LineSensor
{
public:
	LineSensor();
	void test();
	int * rawarray();
	int byteprocessed(int);

	void calibrate();
	void calibrate_show();
	void calibrate_reset();

private:
	int  cal_min[8];
	int  cal_max[8];
};

#endif