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
