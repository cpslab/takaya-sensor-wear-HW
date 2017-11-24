#include "BME280_MOD-1022.h"

#include <Wire.h>

// Arduino needs this to pring pretty numbers

void printFormattedFloat(float x, uint8_t precision) {
char buffer[10];

  dtostrf(x, 7, precision, buffer);
  Serial.print(buffer);

}

void printCompensatedMeasurements(void) {

float temp, humidity,  pressure, pressureMoreAccurate;
double tempMostAccurate, humidityMostAccurate, pressureMostAccurate;
char buffer[80];

  temp      = BME280.getTemperature();
  humidity  = BME280.getHumidity();
  pressure  = BME280.getPressure();
  
  pressureMoreAccurate = BME280.getPressureMoreAccurate();  // t_fine already calculated from getTemperaure() above
  
  tempMostAccurate     = BME280.getTemperatureMostAccurate();
  humidityMostAccurate = BME280.getHumidityMostAccurate();
  pressureMostAccurate = BME280.getPressureMostAccurate();
  Serial.println("                Good  Better    Best");
  Serial.print("Temperature  ");
  printFormattedFloat(temp, 2);
  Serial.print("         ");
  printFormattedFloat(tempMostAccurate, 2);
  Serial.println();
  
  Serial.print("Humidity     ");
  printFormattedFloat(humidity, 2);
  Serial.print("         ");
  printFormattedFloat(humidityMostAccurate, 2);
  Serial.println();

  Serial.print("Pressure     ");
  printFormattedFloat(pressure, 2);
  Serial.print(" ");
  printFormattedFloat(pressureMoreAccurate, 2);
  Serial.print(" ");
  printFormattedFloat(pressureMostAccurate, 2);
  Serial.println();
}


// setup wire and serial

void setup()
{
  Wire.begin();
  Serial.begin(115200);

}

// main loop

void loop()
{

  uint8_t chipID;
  
  Serial.println("Welcome to the BME280 MOD-1022 weather multi-sensor test sketch!");
  Serial.println("Embedded Adventures (www.embeddedadventures.com)");
  chipID = BME280.readChipId();
  
  // find the chip ID out just for fun
  Serial.print("ChipID = 0x");
  Serial.print(chipID, HEX);
  
 
  // need to read the NVM compensation parameters
  BME280.readCompensationParams();
  
  // Need to turn on 1x oversampling, default is os_skipped, which means it doesn't measure anything
  BME280.writeOversamplingPressure(os1x);  // 1x over sampling (ie, just one sample)
  BME280.writeOversamplingTemperature(os1x);
  BME280.writeOversamplingHumidity(os1x);
  
  
  BME280.writeMode(smNormal);
   
  while (1) {
    
    
    while (BME280.isMeasuring()) {


    }
    
    // read out the data - must do this before calling the getxxxxx routines
    BME280.readMeasurements();
    printCompensatedMeasurements();

    delay(1000);  // do this every 5 seconds
    Serial.println();
  }
}

