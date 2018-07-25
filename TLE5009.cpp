
/*
	TLE5009 E series sensor library
	From the datasheet:
	"The TLE5009 is an angle sensor with analog outputs. It detects the orientation of a magnetic field by measuring
	sine and cosine angle components with Giant Magneto Resistance (GMR) elements. It provides analog sine and
	cosine output voltages that describe the magnet angle in a range of 0 to 360°."

  Written by Bill Bigge for Creative Robotics Ltd.
	Distrubuted under the FreeBSD License:
	
	Copyright (c) 2018, Bill Bigge
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
		 list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
		 this list of conditions and the following disclaimer in the documentation
		 and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	The views and conclusions contained in the software and documentation are those
	of the authors and should not be interpreted as representing official policies,
	either expressed or implied, of the TLE5009 library project.
*/
#include "TLE5009.h"


void TLE5009::begin(int sine_Pin, int cosine_Pin, int vgmr_Pin){
	sinP_PIN = sine_Pin;
	cosP_PIN = cosine_Pin;
	vgmd_PIN = vgmr_Pin;
	this->reset();
}
//---------------------------------------------------------------------------------------------

void TLE5009::begin(int sineP_Pin, int cosineP_Pin, int sineN_Pin, int cosineN_Pin, int vgmr_Pin){
	sinP_PIN = sineP_Pin;
	cosP_PIN = cosineP_Pin;
	sinN_PIN = sineN_Pin;
	cosN_PIN = cosineN_Pin;
	vgmd_PIN = vgmr_Pin;
	this->reset();
	differentialMode = DIFFERENTIAL;
}
//---------------------------------------------------------------------------------------------


void TLE5009::debugPrint(){
	Serial.print("rSinP,"); Serial.print(sinP);
	Serial.print(",rSinN,"); Serial.print(sinN);
	Serial.print(",rCosP,"); Serial.print(cosP);
	Serial.print(",rCosN,"); Serial.print(cosN);
	
	Serial.print(",MaxCos,"); Serial.print(maxCos);
	Serial.print(",MinCos,"); Serial.print(minCos);
	Serial.print(",MaxSin,"); Serial.print(maxSin);
	Serial.print(",MinSin,"); Serial.print(minSin);
	
	
	Serial.print(",sinD,"); Serial.print(sinD);
	Serial.print(",cosD,"); Serial.print(cosD);
	
	Serial.print(",sine,"); Serial.print(sine, 4);
	Serial.print(",cosine,"); Serial.print(cosine, 4);

	Serial.print(",sinScale,"); Serial.print(sinScale, 4);
	Serial.print(",cosScale,"); Serial.print(cosScale, 4);
	Serial.print(",sinVShift,"); Serial.print(sinVShift, 4);
	Serial.print(",cosVShift,"); Serial.print(cosVShift, 4);

	Serial.print(",sinHD,"); Serial.print(sinHalfDifference, 4);
	Serial.print(",cosHD,"); Serial.print(cosHalfDifference, 4);
	Serial.println();
	
}
//---------------------------------------------------------------------------------------------



void TLE5009::setCalibration(int sMin, int sMax, int cMin, int cMax){
	maxCos = cMax;
	minCos = cMin;
	maxSin = sMax;
	minSin = sMin;
	udateScale();
}
//---------------------------------------------------------------------------------------------

void TLE5009::update(){
	readSensors();
	if(calibrationEnabled){
		updateMinMax();
		udateScale();
	}
	
	if(differentialMode){
		sinD = sinP - sinN;
		cosD = cosP - cosN;
		sine 		= (float)sinD*sinScale;
		cosine 	= (float)cosD*cosScale;
	}else{
		sine 		= ((float)sinP - sinVShift)*sinScale;
		cosine 	= ((float)cosP - cosVShift)*cosScale;
	}
	sine = clampS(sine, 1.0);
	cosine = clampS(cosine, 1.0);
	oldAngle = angle;
	angle = atan2(sine, cosine);
	//angle = debugVar1;
	updateDeltas();
	updateRevolutions();
}
//---------------------------------------------------------------------------------------------
	
float TLE5009::getAngle(){
	if(outputDegrees) return angle*TODEGREES;
	else return angle;
}
//---------------------------------------------------------------------------------------------
	
float TLE5009::getDelta(){
	if(outputDegrees) return angleDelta*TODEGREES;
	else return angleDelta;
}
//---------------------------------------------------------------------------------------------
	

void TLE5009::resetRevolutions(){
	//return the number of revolutions since reset or start
	revCounts = 0;
	revolutions = 0.0;
	absAng = 0.0;
	updateRevolutions();
}
//---------------------------------------------------------------------------------------------
	
void TLE5009::reset(){
	cosP = 0;
	sinP = 0;
	cosN = 0;
	sinN = 0;
	vgmr = 0;
	
	maxCos = 0;
	minCos = MAXINT;
	maxSin = 0;
	minSin = MAXINT;
	
	sinD = 0;
	cosD = 0;
	
	sine = 0.0;
	cosine = 0.0;
	
	angle = 0.0;
	oldAngle = 0.0;
	angleDelta = 0.0;
	
	revCounts = 0;
	revolutions = 0.0;
	absAng = 0.0;
	
	lastUpdateTime = millis();
	
	sinHalfDifference = 0.0;
	cosHalfDifference = 0.0;
	
	sinScale = 0.0;
	cosScale = 0.0;
	sinVShift = 0.0;
	cosVShift = 0.0;
	
	angleOffset = 0.0;
	
	outputDegrees = RADIANS; //false = radians
	calibrationEnabled = false;
	differentialMode = SINGLE_ENDED;
}
//---------------------------------------------------------------------------------------------
void TLE5009::resetCalibration(){
	maxCos = 0;
	minCos = MAXINT;
	maxSin = 0;
	minSin = MAXINT;
	sinHalfDifference = 0.0;
	cosHalfDifference = 0.0;
	
	sinScale = 0.0;
	cosScale = 0.0;
	sinVShift = 0.0;
	cosVShift = 0.0;
}
//---------------------------------------------------------------------------------------------
void TLE5009::readSensors(){
	if(sinP_PIN != -1) sinP = analogRead(sinP_PIN);
	if(cosP_PIN != -1) cosP = analogRead(cosP_PIN);
	if(sinN_PIN != -1) sinN = analogRead(sinN_PIN);
	if(cosN_PIN != -1) cosN = analogRead(cosN_PIN);
	if(vgmd_PIN != -1) vgmr = analogRead(vgmd_PIN);
}
//---------------------------------------------------------------------------------------------

void TLE5009::updateMinMax(){
	if(differentialMode){
		if(maxSin < sinD) maxSin = sinD;
		if(minSin > sinD) minSin = sinD;
		
		if(maxCos < cosD) maxCos = cosD;
		if(minCos > cosD) minCos = cosD;
		
	}else{
		if(maxSin < sinP) maxSin = sinP;
		if(minSin > sinP) minSin = sinP;
		
		if(maxCos < cosP) maxCos = cosP;
		if(minCos > cosP) minCos = cosP;
	}
}
//---------------------------------------------------------------------------------------------

void TLE5009::udateScale(){
	sinHalfDifference = ( (float)maxSin-(float)minSin ) / 2.0;
	cosHalfDifference = ( (float)maxCos-(float)minCos ) / 2.0;
	
	sinVShift = ( (float)maxSin+(float)minSin ) / 2.0;
  sinScale = 1.0/sinHalfDifference;
	
	cosVShift = ( (float)maxCos+(float)minCos ) / 2.0;
  cosScale = 1.0/cosHalfDifference;
}
//---------------------------------------------------------------------------------------------

void TLE5009::updateDeltas(){
	//Currently does NOT handle angle wrap around.
	float timeDelta = (float)( millis() - lastUpdateTime ) / 1000;
	float angleDifference;
	angleDifference = angleDiff(angle,oldAngle);
	angleDelta = -(angleDifference/timeDelta);
	lastUpdateTime = millis();
}
//---------------------------------------------------------------------------------------------

void TLE5009::updateRevolutions(){
	//update the rev counter
	//check for variable wrap - use this as the point to tick one revolution
	if(angle < 0.0 && oldAngle > 0.0 && angleDelta > 0.0) revCounts++;
	if(angle > 0.0 && oldAngle < 0.0 && angleDelta < 0.0) revCounts--;
	
	//convert angle to an unsigned value
	absAng = constrain(angle + PI, 0.0, TWOPI);
	angleFraction = absAng/TWOPI;
	revolutions = revCounts + angleFraction;
}
//---------------------------------------------------------------------------------------------

float TLE5009::clampS(float var, float limit){
	//constrain to the limit value
	if(var > limit)  			return limit;
	else if(var < -limit) return -limit;
	else 									return var;
}

//copied from stack overflow ..!
//Code for handling wrap arounds with floating point derivative of angle

float TLE5009::constrainAngle(float x){
    x = fmod(x + PI, TWOPI);
    if (x < 0)
        x += TWOPI;
    return x - PI;
}


float TLE5009::angleDiff(float a,float b){
    float dif = fmod(b - a + PI, TWOPI);
    if (dif < 0)
        dif += TWOPI;
    return dif - PI;
}

float TLE5009::bisectAngle(float a,float b){
    return constrainAngle(a + angleDiff(a,b) * 0.5);
}

