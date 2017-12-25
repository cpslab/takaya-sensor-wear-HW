void readTweLite() {
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}

