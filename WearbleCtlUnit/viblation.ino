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
    //    if (vib[0]) {
    //      vibrationOn();
    //      neopixelColor(0, 255, 0);
    //      delay(1000);
    //      vibrationOff();
    //      neopixelOff();
    //      vib[0] = !vib[0];
    //    }

    if (vib[0]) {
      vibrationOn();
      delay(50 * PIXELS);
      vibrationOff();
    } else {
      vibrationOff();
    }
    if (vib[1]) {
      vibrationOn();
    } else {
      vibrationOff();
    }
    if (vib[2]) {  //通知ユニット
      vibrationOn();
    } else {
      vibrationOff();
    }
    delay(1);
  }
}
