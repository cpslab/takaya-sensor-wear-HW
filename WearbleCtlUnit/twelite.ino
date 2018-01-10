void readTweLite() {
  if (Serial1.available()) {
    char c = Serial1.read();
    Serial.write(c);
    if (c == '\n')vib = true;
  }
}

