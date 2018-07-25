

void checkSerial(){
  int inByte;
  if(Serial.available()){
    inByte = Serial.read();
    switch(inByte){
      case 'p':
        //toggle printing
        printing = !printing;
        break;
      case 'c':
        //calibrate toggle
        mySensor.toggleCalibrateEnable();
        if(mySensor.isCalibrating()) 
        {
          mySensor.resetCalibration();
          Serial.println("Calibrating");
        }
        else Serial.println("Finished Calibrating");
        break;
      case 'u':
        //toggle unit mode
        angMode = !angMode;
        mySensor.setAngleMode(angMode);
        if(mySensor.getAngleMode()) Serial.println("Degrees");
        else                   Serial.println("Radians");
        break;
      case 'm':
        //get measurement mode
        if(mySensor.getMeasurementMode()) Serial.println("Double ended");
        else                         Serial.println("Single ended");
        break;
    }
  }
}
