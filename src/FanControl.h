#pragma once

class FanControl{
  
  int pinFan;
  int newTemp;
  int newTT;
  int newMIS;
  int newMXS;
  int xPWM;
  
  public:

  //0 Pin Nummer, 1 Traget Temperatur Lüfter, 2 minimal Geschwindigkeit Lüfter, 3 maximaler Speed Lüfter
  FanControl(int pPin, int pTargetTemp, int pMinSpeed, int pMaxSpeed){
    pinFan = pPin;
    newTT = pTargetTemp;
    newMIS = pMinSpeed;
    newMXS = pMaxSpeed;
    xPWM = pMinSpeed; // init
    pinMode(pinFan, OUTPUT);
  }
  
  int fSpeed(int pTemp){

    newTemp = pTemp;

    if(newTemp > newTT){
      xPWM++;
    }
    if(newTemp < newTT){
      xPWM--;
    }
    xPWM = constrain(xPWM, newMIS, newMXS);
    analogWrite(pinFan, xPWM);
    return xPWM;
  }
  
  private:
    
};
