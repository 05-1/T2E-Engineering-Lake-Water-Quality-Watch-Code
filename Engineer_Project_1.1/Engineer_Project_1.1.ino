#include "TDS.h" // Include TDS sensor class
#include "therm.h" // Include thermistor class
#include "PH.h" // Include pH sensor class
#include <SPI.h> // SPI library for SD communication
#include <SD.h> // SD card library

phsens phsens; // Create instance of pH sensor
therm therm; // Create instance of thermistor
TDS TDS; // Create instance of TDS sensor

const int chipSelect = 10; // Chip select pin for SD card module
File myFile; // File object for SD operations

void setup()
{
  Serial.begin(9600); // Start serial communication at 9600 baud

  // Wait for Serial Monitor to connect (only needed for boards with native USB)
  while (!Serial);

  Serial.print("Initializing SD card...");

  // Try to initialize SD card with chip select pin
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true); // Halt program if SD init fails
  }

  Serial.println("initialization done."); // SD initialized successfully
}

void loop()
{
  // Collect 5 samples from each sensor
  for (int i = 0; i < 5; i++) {
    therm.Therm_run(); // Read temperature
    TDS.TDS_run(); // Read TDS value
    phsens.ph(); // Read pH value
  }

  delay(1000); // Wait 1 second before logging

  // Open file to write data
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to test.txt..."); // Notify start of write
    myFile.println("PH:"); // Write pH label
    myFile.println(pHValue); // Write pH value
    myFile.println("TDS:"); // Write TDS label
    myFile.println(tdsValue); // Write TDS value
    myFile.println("temperature:"); // Write temperature label
    myFile.println(steinhart); // Write temperature value

    myFile.close(); // Save and close file
    Serial.println("done."); // Notify write complete
  } else {
    Serial.println("error opening test.txt"); // Error if file can't be opened
  }
}
