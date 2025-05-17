#pragma once

#include <SPI.h> // Include SPI library
#include <SD.h> // Include SD card library
#include "HardwareSerial.h" // For serial communication
#include "Arduino.h" // Core Arduino definitions

#define SensorPin A1            // pH sensor analog output connected to Arduino analog pin A1
#define Offset .3               // Voltage-to-pH calibration offset
#define samplingInterval 20     // Milliseconds between each sample
#define printInterval 800       // Milliseconds between each Serial print
#define ArrayLenth  5           // Number of samples to store

int pHArray[ArrayLenth];        // Array to store recent pH analog readings
int pHArrayIndex = 0;           // Index for circular buffer of pH samples
float x = 0.0;                  // Temporary analog reading for debugging
static float pHValue, voltage;  // Final computed pH value and voltage

// Class to encapsulate pH sensor logic
class phsens {

public:
  phsens(){
    // Constructor does nothing (can be used for setup if needed)
  }

  // Function to handle reading and printing pH data
  void ph() {
    static unsigned long samplingTime = millis(); // Track time since last sample
    static unsigned long printTime = millis();    // Track time since last print

    // Sample the pH sensor at defined interval
    if (millis() - samplingTime > samplingInterval) {
      pHArray[pHArrayIndex++] = analogRead(SensorPin); // Read and store analog value
      if (pHArrayIndex == ArrayLenth) pHArrayIndex = 0; // Wrap index if necessary

      // Calculate voltage and pH from averaged analog readings
      voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
      pHValue = 3.5 * voltage + Offset; // Convert voltage to pH using scaling + offset

      samplingTime = millis(); // Reset sample timer
    }

    // Print data to Serial every 800 ms
    if (millis() - printTime > printInterval) {
      x = analogRead(SensorPin); // Read raw analog value for reference
      Serial.print("analog pin reading:");
      Serial.println(x);
      Serial.print("Voltage:");
      Serial.print(voltage, 2);
      Serial.print("    pH value: ");
      Serial.println(pHValue, 2);

      printTime = millis(); // Reset print timer
    }
  }

  // Helper function to average an array with outlier filtering
  double avergearray(int* arr, int number) {
    int i;
    int max, min;
    double avg;
    long amount = 0;

    if (number <= 0) {
      Serial.println("Error number for the array to avraging!/n");
      return 0;
    }

    // If few elements, average them all directly
    if (number < 5) {
      for (i = 0; i < number; i++) {
        amount += arr[i];
      }
      avg = amount / number;
      return avg;
    } else {
      // Find initial min and max
      if (arr[0] < arr[1]) {
        min = arr[0]; max = arr[1];
      } else {
        min = arr[1]; max = arr[0];
      }

      // Accumulate values excluding the current min and max
      for (i = 2; i < number; i++) {
        if (arr[i] < min) {
          amount += min; // Discard old min
          min = arr[i];
        } else if (arr[i] > max) {
          amount += max; // Discard old max
          max = arr[i];
        } else {
          amount += arr[i]; // Value is between min and max
        }
      }

      avg = (double)amount / (number - 2); // Exclude 2 outliers
    }

    return avg;
  }
};
