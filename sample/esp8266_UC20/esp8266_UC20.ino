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
