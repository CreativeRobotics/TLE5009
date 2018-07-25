#include <TLE5009.h>
//Calibration sketch for single ended operation
TLE5009 mySensor;
#define SINEPPIN    A3
#define COSINEPPIN  A1
#define VGMRPIN     A4
bool angMode = 0; //units in radians
bool calibrating = true;
void setup() {
  mySensor.begin(SINEPPIN, COSINEPPIN, VGMRPIN);
  Serial.begin(115200);
  delay(2000);
}

void loop() {
  mySensor.update();
  Serial.println("send any character to start calibration");
  Serial.println("Make sure the sensor magnet is slowly rotating whilst the system calibrates");
  waitForCommand();
  
  mySensor.resetCalibration();
  mySensor.enableCalibrate();
  Serial.println("Calibration started!");
  Serial.println("Make sure the sensor magnet is slowly rotating whilst the system calibrates");
  Serial.println("When the sensor has rotated a few times, send another character to end calibration");
  while(calibrating){
    mySensor.update();
    delay(1);
    calibrating = !checkForCommand(); //if a char is recieved then end calibration
  }

  mySensor.disableCalibrate();
  Serial.println("");
  Serial.println("Calibration Values are:");
  Serial.print(" Sine Min = ");
  Serial.println(mySensor.getSineMin());
  Serial.print(" Sine Max = ");
  Serial.println(mySensor.getSineMax());
  Serial.print(" Cosine Min = ");
  Serial.println(mySensor.getCosineMin());
  Serial.print(" Cosine Max = ");
  Serial.println(mySensor.getCosineMax());
  Serial.println("");
  
  Serial.println("Copy and paste code:");
  Serial.println("");
  Serial.print("  mySensor.setCalibration(");
  Serial.print(mySensor.getSineMin());
  Serial.print(", ");
  Serial.print(mySensor.getSineMax());
  Serial.print(", ");
  Serial.print(mySensor.getCosineMin());
  Serial.print(", ");
  Serial.print(mySensor.getCosineMax());
  Serial.println( ");" );

  Serial.println("");
  
  Serial.println("Send a character to start over");
  waitForCommand();
}


