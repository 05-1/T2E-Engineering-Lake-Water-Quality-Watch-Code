#include "TDS.h"
#include "therm.h"
#include "PH.h"
#include <SPI.h>
#include <SD.h>
phsens phsens;
therm therm;
TDS TDS;

const int chipSelect = 10;
File myFile;

void setup()
{
  Serial.begin(9600);
    // wait for Serial Monitor to connect. Needed for native USB port boards only:
  while (!Serial);

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true);
  }

  Serial.println("initialization done.");



  


}

void loop()
{
for (int i = 0; i < 5; i++) {
therm.Therm_run();
TDS.TDS_run();
phsens.ph();

}
delay(1000);
myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("PH:");
    myFile.println(pHValue);
    myFile.println("TDS:");
    myFile.println(tdsValue);
    myFile.println("temperature:");
    myFile.println(steinhart);


    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}