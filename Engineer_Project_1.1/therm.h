#include "pins_arduino.h"
#pragma once

#include <SPI.h>
#include <SD.h>

// Analog pin connected to the thermistor
#define THERMISTORPIN A2
#define THERMISTORNOMINAL 10000        // Nominal resistance at 25°C
#define TEMPERATURENOMINAL 25          // Nominal temperature in Celsius
#define NUMSAMPLES 5                   // Number of samples for averaging
#define BCOEFFICIENT 3950              // Beta coefficient of thermistor
#define SERIESRESISTOR 10000           // Series resistor value

static float steinhart;               // Global temperature result variable

int samples[NUMSAMPLES];              // Buffer for thermistor readings

// Class for reading and processing thermistor values
class therm {
public:
  therm() {
    analogReference(AR_EXTERNAL);     // Use external analog reference voltage
  }

  void Therm_run() {
    uint8_t i;
    float average;

    // Take multiple samples from thermistor
    for (i = 0; i < NUMSAMPLES; i++) {
      samples[i] = analogRead(THERMISTORPIN);
      delay(10);
    }

    // Compute average analog reading
    average = 0;
    for (i = 0; i < NUMSAMPLES; i++) {
      average += samples[i];
    }
    average /= NUMSAMPLES;

    Serial.print("Average analog reading ");
    Serial.println(average);

    // Convert ADC value to resistance
    average = 1023 / average - 1;
    average = SERIESRESISTOR / average;
    Serial.print("Thermistor resistance ");
    Serial.println(average);

    // Apply Steinhart-Hart equation to convert resistance to temperature
    steinhart = average / THERMISTORNOMINAL;
    steinhart = log(steinhart);
    steinhart /= BCOEFFICIENT;
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);
    steinhart = 1.0 / steinhart;
    steinhart -= 273.15;  // Convert from Kelvin to Celsius

    Serial.print("Temperature ");
    Serial.print(steinhart);
    Serial.println(" *C");
  }
};
