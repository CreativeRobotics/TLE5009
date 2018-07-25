
void waitForCommand(){
  while(!Serial.available()){}
  while(Serial.available()){
    int inByte = Serial.read();
    //clear the serial buffer
  }
}

bool checkForCommand(){
  if(Serial.available()){
    while(Serial.available()){
      int inByte = Serial.read();
      //clear the serial buffer
    }
    return true;
  }
  return false;
}

