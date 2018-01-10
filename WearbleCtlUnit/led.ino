void initNeopixel() {
  strip1.begin();
  strip2.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.show(); // Initialize all pixels to 'off'
  neopixelOff();
}
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel((i + j) & 255));
      strip2.setPixelColor(i, Wheel((i + j) & 255));

    }
    strip1.show();
    strip2.show();
    delay(wait);
  }
}

void neopixelColor(int r, int g, int b) {
  for (int i = 0; i < strip1.numPixels(); i++) {
    strip1.setPixelColor(i, strip1.Color(r, g, b));
    strip2.setPixelColor(i, strip2.Color(r, g, b));
  }
  strip1.show();
  strip2.show();
}

void neopixelOff() {
  for (int i = 0; i < strip1.numPixels(); i++) {
    strip1.setPixelColor(i, strip1.Color(0, 0, 0));
    strip2.setPixelColor(i, strip2.Color(0, 0, 0));
  }
  strip1.show();
  strip2.show();
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip1.numPixels(); i++) {
    strip1.setPixelColor(i, c);
    strip1.show();
    strip2.setPixelColor(i, c);
    strip2.show();
    delay(wait);
  }
}

void led_t(void* arg) {
  while (1) {
    if (led) {
      colorWipe(strip1.Color(255, 0, 0), 50);
      colorWipe(strip2.Color(255, 0, 0), 50);
      neopixelOff();
      led = !led;
    }
    delay(1);
  }
}
