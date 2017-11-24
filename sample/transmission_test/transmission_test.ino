int16_t accX, accY, accZ, magX, magY, magZ, gyoX, gyoY, gyoZ, tmp, hum, prs;
float lat, lng;
byte flag;
byte output[33]; //24+8+1
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start");

  accX = 9999;
  accY = 9999;
  accZ = 9999;
  output[0] = byte(accX);
  output[2] = byte(accY);
  output[4] = byte(accZ);

  magX = 8888;
  magY = 8888;
  magZ = 8888;
  output[6] = byte(magX);
  output[8] = byte(magY);
  output[10] = byte(magZ);

  gyoX = 7777;
  gyoY = 7777;
  gyoZ = 7777;
  output[12] = byte(gyoX);
  output[14] = byte(gyoY);
  output[16] = byte(gyoZ);

  tmp = 2222;
  hum = 2222;
  prs = 123456;
  output[18] = byte(tmp);
  output[20] = byte(hum);
  output[22] = byte(prs);

  lat = 3541.1493;
  lng = 13945.3994;
  flag = B00000001;
  output[24] = byte(lat);
  output[28] = byte(lng);
  output[32] = byte(flag);
  for (int i = 0; i < 33; i++) {
    Serial.print(output[i],BIN);
  }
//  Serial.println((BIN)accX);
//  Serial.println(accX,BIN);
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
