#include "BME280_MOD-1022.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Wire.h>

unsigned long lastStreamTime = 0;     //To store the last streamed time stamp
const int streamPeriod = 5000;        //　間欠動作時間


int16_t accX, accY, accZ, magX, magY, magZ, gyoX, gyoY, gyoZ, tmp, hum, prs;
float lat, lng, eX, eY, eZ;;
byte flag;
//char output[33]; //24+8+1

HardwareSerial  Serial2(1); // RX, TX

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
