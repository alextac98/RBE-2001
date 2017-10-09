#ifndef _LINEFOLLOWER_h
#define _LINEFOLLOWER_h

class LineSensor{
public:
	LineSensor();				//constructor

	void test();				//used for testing
	unsigned int *rawArray();	//returns RAW values
	int *processedArray();		//returns 0 or 1 for line
	float avgLinePos();			//returns line position from 1 to -1 (L to R)

	void calibrate();
	void calibrate_show();
	void calibrate_reset();

	

private:
	int  calibrationPoint = 200;
	float scaleNumber(float x, float in_min, float in_max, float out_min, float out_max);
	unsigned int* raw = new unsigned int[9];

};



#endif