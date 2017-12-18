HardwareSerial Serial2(1); // RX, TX
void setup()
{
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
}
void loop()
{
  if (Serial2.available())
    Serial.write(Serial2.read());
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '/') {
      Serial.println("enable");
      enable();
    }
    else {
      Serial2.write(c);
    }
  }
}

void enable() {
  digitalWrite(12, HIGH);
  delay(1000);
  digitalWrite(12, LOW);
}

