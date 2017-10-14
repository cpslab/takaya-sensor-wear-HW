/**************************************************************/
/*  esp8266_UC_20 with UC20-G 3G/LTE module                   */
/*  For TAKAYA Work Wear Project                              */
/*  Base sketch by Naoki                                      */
/*                                                            */
/*  Author:    Naoki Nonaka                                   */
/*  Date:      2017/10/13.                                    */
/*  MPU : ESP-8266                                            */
/*                                                            */
/**************************************************************/

#include <SoftwareSerial.h>
SoftwareSerial mySerial(5,4); // RX, TX
void setup()
{
 Serial.begin(115200);
 mySerial.begin(115200);
}
void loop()
{
 if(mySerial.available())
 Serial.write(mySerial.read());
 if (Serial.available())
 mySerial.write(Serial.read());
}
