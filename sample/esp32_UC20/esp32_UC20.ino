HardwareSerial Serial2(1); // RX, TX
void setup()
{
 Serial.begin(115200);
 Serial2.begin(115200, SERIAL_8N1, 16, 17);
}
void loop()
{
 if(Serial2.available())
 Serial.write(Serial2.read());
 if (Serial.available())
 Serial2.write(Serial.read());
}
