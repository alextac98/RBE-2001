#ifndef _LINEFOLLOWER_h
#define _LINEFOLLOWER_h

class LineSensor{
public:
	LineSensor();
	void test();
	unsigned int *rawArray();
	int *processedArray();

	void calibrate();
	void calibrate_show();
	void calibrate_reset();

private:
	int  calibrationPoint = 200;

};

#endif