#include <TLE5009.h>

TLE5009 mySensor;
bool printing = true;
#define SINEPPIN    A3
#define COSINEPPIN  A1
#define SINENPIN    A2
#define COSINENPIN  A0
#define VGMRPIN     A4
bool angMode = 0;
void setup() {
  //initialise with all inputs needed for differential input but manually set the mode to single ended
  //Using my own calibration values obtained from a calibration run.
  mySensor.begin(SINEPPIN, COSINEPPIN, SINENPIN, COSINENPIN, VGMRPIN);

  mySensor.setCalibration(-622, 616, -603, 619);
 //int sMin, int sMax, int cMin, int cMax
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


