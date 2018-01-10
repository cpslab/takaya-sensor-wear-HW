void initVibration() {
  pinMode(VIBRATOR1, OUTPUT);
  digitalWrite(VIBRATOR1, LOW);
  pinMode(VIBRATOR2, OUTPUT);
  digitalWrite(VIBRATOR2, LOW);
}

void vibrationOn() {
  digitalWrite(VIBRATOR1, HIGH);
  digitalWrite(VIBRATOR2, HIGH);
}

void vibrationOff() {
  digitalWrite(VIBRATOR1, LOW);
  digitalWrite(VIBRATOR2, LOW);
}

void vib_t(void* arg) {
  while (1) {
    if (vib) {
      vibrationOn();
      neopixelColor(0, 255, 0);
      delay(1000);
      vibrationOff();
      neopixelOff();
      vib = !vib;
    }
    delay(1);
  }
}
