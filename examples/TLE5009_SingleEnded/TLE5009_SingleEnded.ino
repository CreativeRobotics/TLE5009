#include <TLE5009.h>

TLE5009 mySensor;
bool printing = true;
#define SINEPPIN    A3
#define COSINEPPIN  A1
#define VGMRPIN     A4
bool angMode = 0;
void setup() {
  mySensor.begin(SINEPPIN, COSINEPPIN, VGMRPIN);
  //Calibration Values are:
  //Sine Min = 201
  //Sine Max = 820
  //Cosine Min = 213
  //Cosine Max = 825
  mySensor.setCalibration(201, 820, 213, 825); //int sMin, int sMax, int cMin, int cMax
  Serial.begin(115200);
}

void loop() {
  mySensor.update();
  
  if(printing){
    Serial.print(mySensor.getAngle(), 6);
    Serial.print(",");
    Serial.print(mySensor.getDelta(), 6);
    Serial.print(",");
    Serial.println(mySensor.getRevolutions(), 2);
  }
  delay(50);
  checkSerial();
}


