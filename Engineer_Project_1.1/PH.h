#pragma once

#include <SPI.h>
#include <SD.h>
#include "HardwareSerial.h"
#include "Arduino.h"

#define SensorPin A1            //pH meter Analog output to Arduino Analog Input 0
#define Offset .3           //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  5    //times of collection



int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;    
float x = 0.0;
static float pHValue,voltage;  

class phsens {

public:
  phsens(){

  }

void ph() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();

  

  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    x = analogRead(SensorPin);
    Serial.print("analog pin reading:");
    Serial.println(x);
    Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
    Serial.println(pHValue,2);

    


        printTime=millis();
  }
  

}


double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}



};