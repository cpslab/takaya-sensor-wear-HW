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

//#include <SoftwareSerial.h>
HardwareSerial  Serial1(2); // RX, TX
void setup()
{
// Serial.begin(9600);
//  Serial1.begin(9600, SERIAL_8N1, 18, 5);
 Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 32, 33);
}
void loop()
{
 if(Serial1.available())
 Serial.write(Serial1.read());
 if (Serial.available())
 Serial1.write(Serial.read());
}
