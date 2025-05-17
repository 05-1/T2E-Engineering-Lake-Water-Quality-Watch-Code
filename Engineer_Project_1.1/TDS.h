#pragma once

#include <SPI.h>
#include <SD.h>
#include "Arduino.h"
#include "therm.h"

#define TdsSensorPin A0         // Analog pin connected to TDS sensor
#define VREF 5.0                // Analog reference voltage for ADC
#define SCOUNT  5               // Number of samples to average

// Buffers and index variables for reading and processing analog values
int analogBuffer[SCOUNT];       
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
static float averageVoltage = 0, tdsValue = 0;

// TDS class handles total dissolved solids sensor data acquisition and conversion
class TDS{
  public:
  TDS(){
    pinMode(TdsSensorPin, INPUT);   // Set TDS sensor pin as input
  }

  void TDS_run()
  {
    // Sample analog input every 40 ms
    static unsigned long analogSampleTimepoint = millis();
    if (millis() - analogSampleTimepoint > 40U) {
      analogSampleTimepoint = millis();
      analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);  // Read analog value
      analogBufferIndex++;
      if (analogBufferIndex == SCOUNT)
        analogBufferIndex = 0;  // Reset index when buffer is full
    }

    // Process and print every 800 ms
    static unsigned long printTimepoint = millis();
    if (millis() - printTimepoint > 800U) {
      printTimepoint = millis();
      
      // Copy analog buffer for filtering
      for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
        analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
      
      // Apply median filter and convert to voltage
      averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0;

      // Apply temperature compensation
      float compensationCoefficient = 1.0 + 0.02 * (steinhart - 25.0);
      float compensationVolatge = averageVoltage / compensationCoefficient;

      // Convert voltage to TDS value using polynomial regression formula
      tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge 
                 - 255.86 * compensationVolatge * compensationVolatge 
                 + 857.39 * compensationVolatge) * 0.5;

      // Output TDS value
      Serial.print("TDS----Value:");
      Serial.print(tdsValue, 0);
      Serial.println("ppm");
    }
  }

  // Function to return median of sample array
  int getMedianNum(int bArray[], int iFilterLen)
  {
    int bTab[iFilterLen];
    for (byte i = 0; i < iFilterLen; i++)
      bTab[i] = bArray[i];

    int i, j, bTemp;
    for (j = 0; j < iFilterLen - 1; j++) {
      for (i = 0; i < iFilterLen - j - 1; i++) {
        if (bTab[i] > bTab[i + 1]) {
          bTemp = bTab[i];
          bTab[i] = bTab[i + 1];
          bTab[i + 1] = bTemp;
        }
      }
    }

    // Return median
    if ((iFilterLen & 1) > 0)
      bTemp = bTab[(iFilterLen - 1) / 2];
    else
      bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;

    return bTemp;
  }
};
