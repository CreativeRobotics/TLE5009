//TLE5009 E series sensor library
/*
"The TLE5009 is an angle sensor with analog outputs. It detects the orientation of a magnetic field by measuring
sine and cosine angle components with Giant Magneto Resistance (GMR) elements. It provides analog sine and
cosine output voltages that describe the magnet angle in a range of 0 to 360°."
*/

#ifndef TLE5009_H
#define TLE5009_H
#include <inttypes.h>
#include <math.h>
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif





#define DEGREES 			true
#define RADIANS 			false
#define DIFFERENTIAL 	true
#define SINGLE_ENDED 	false

#define PI 		3.1415926535897932384626433832795
#define TWOPI 	6.283185307179586476925286766559
#define TODEGREES 57.295779513082320876798154814105
#define MAXINT 32767
class TLE5009{
	public:
	TLE5009(void){}
	void begin(int sine_Pin, int cosine_Pin, int vgmr_Pin);
	void begin(int sineP_Pin, int cosineP_Pin, int sineN_Pin, int cosineN_Pin, int vgmr_Pin);
	void setCalibration(int sMin, int sMax, int cMin, int cMax);
	void update();
	void enableCalibrate() 				{calibrationEnabled = true;}
	void disableCalibrate() 			{calibrationEnabled = false;}
	void toggleCalibrateEnable() 	{calibrationEnabled = !calibrationEnabled;}
	bool isCalibrating() 					{return calibrationEnabled;}
	void resetCalibration();
	
	void setAngleMode(bool mode)	{outputDegrees = mode;}
	bool getAngleMode() 					{return outputDegrees;}
	void setMeasurementMode(bool mode) 	{differentialMode = mode;}
	bool getMeasurementMode() 		{return differentialMode;}
	
	float getAngle();
	float getDelta();
	float getRevolutions() 				{return revolutions;}
	void resetRevolutions();
	
	float getRevcounts() 					{return revCounts;}
	float getAngleFrac() 					{return angleFraction;}
	float getAbsAng() 						{return absAng;}
	int getSineMin() 							{return minSin;}
	int getSineMax() 							{return maxSin;}
	int getCosineMin() 						{return minCos;}
	int getCosineMax() 						{return maxCos;}
	void debugPrint(); //print most of the private variables to Serial
	
	private:
	void reset();
	void readSensors();
	void updateMinMax();
	void udateScale();
	void updateDeltas();
	void updateRevolutions();
	float clampS(float var, float limit);
	float constrainAngle(float x);
	float angleDiff(float a,float b);
	float bisectAngle(float a,float b);
	
	int sinP_PIN = -1, cosP_PIN = -1, sinN_PIN = -1, cosN_PIN = -1, vgmd_PIN = -1;
	
	
	int cosP, sinP, cosN, sinN;
	int vgmr;
	int maxCos, minCos, maxSin, minSin;
	int sinD, cosD;
	float sine, cosine;
	
	float angle 						= 0.0;
	float oldAngle 					= 0.0;
	float angleDelta 				= 0.0;
	
	float revCounts 				= 0;
	float revolutions 			= 0.0;
	float absAng 						= 0.0;
	float angleFraction 		= 0.0;
	
	unsigned long lastUpdateTime = 0;
	
	float sinHalfDifference = 0.0;
	float cosHalfDifference = 0.0;
	
	float sinScale 					= 0.0;
	float cosScale 					= 0.0;
	float sinVShift 				= 0.0;
	float cosVShift 				= 0.0;
	
	float angleOffset 			= 0.0;
	
	bool outputDegrees 			= false; //false = radians
	bool calibrationEnabled = false;
	bool differentialMode 	= false;
};

#endif