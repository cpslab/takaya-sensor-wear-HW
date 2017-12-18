void printFormattedFloat(float x, uint8_t precision) {
  char buffer[10];

  dtostrf(x, 7, precision, buffer);
  Serial.print(buffer);

}

void getCompensatedMeasurements(void) {

  float temp, humidity,  pressure, pressureMoreAccurate;
  double tempMostAccurate, humidityMostAccurate, pressureMostAccurate; //arduino的にはfloatもdoubleも一緒っぽい？
  char buffer[80];

  temp      = BME280.getTemperature();
  humidity  = BME280.getHumidity();
  pressure  = BME280.getPressure();

//  tmp = temp;
//  hum = humidity;
//  prs = pressure;
  
  pressureMoreAccurate = BME280.getPressureMoreAccurate();  // t_fine already calculated from getTemperaure() above

  tempMostAccurate     = BME280.getTemperatureMostAccurate();
  humidityMostAccurate = BME280.getHumidityMostAccurate();
  pressureMostAccurate = BME280.getPressureMostAccurate();

  tmp = tempMostAccurate;
  hum = humidityMostAccurate;
  prs = pressureMostAccurate;
}

void measureBME280() {
  // need to read the NVM compensation parameters
  BME280.readCompensationParams();

  // Need to turn on 1x oversampling, default is os_skipped, which means it doesn't measure anything
  BME280.writeOversamplingPressure(os1x);  // 1x over sampling (ie, just one sample)
  BME280.writeOversamplingTemperature(os1x);
  BME280.writeOversamplingHumidity(os1x);

  BME280.writeMode(smNormal);

    while (BME280.isMeasuring()) {
    }

    // read out the data - must do this before calling the getxxxxx routines
    BME280.readMeasurements();
    getCompensatedMeasurements();
}


