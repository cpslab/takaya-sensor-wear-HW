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
  //  output[0] = byte(accX);
  //  output[2] = byte(accY);
  //  output[4] = byte(accZ);

  output[0] = accX >> 8;
  output[1] = accX & 0xff;
  output[2] = accY >> 8;
  output[3] = accY & 0xff;
  output[4] = accZ >> 8;
  output[5] = accZ & 0xff;


  magX = 8888;
  magY = 8888;
  magZ = 8888;
  //  output[6] = byte(magX);
  //  output[8] = byte(magY);
  //  output[10] = byte(magZ);
  output[6] = magX >> 8;
  output[7] = magX & 0xff;
  output[8] = magY >> 8;
  output[9] = magY & 0xff;
  output[10] = magZ >> 8;
  output[11] = magZ & 0xff;

  gyoX = 7777;
  gyoY = 7777;
  gyoZ = 7777;
  //  output[12] = byte(gyoX);
  //  output[14] = byte(gyoY);
  //  output[16] = byte(gyoZ);
  output[12] = gyoX >> 8;
  output[13] = gyoX & 0xff;
  output[14] = gyoY >> 8;
  output[15] = gyoY & 0xff;
  output[16] = gyoZ >> 8;
  output[17] = gyoZ & 0xff;

  tmp = 2222;
  hum = 2222;
  prs = 123456;
  //  output[18] = byte(tmp);
  //  output[20] = byte(hum);
  //  output[22] = byte(prs);
  output[18] = tmp >> 8;
  output[19] = tmp & 0xff;
  output[20] = hum >> 8;
  output[21] = hum & 0xff;
  output[22] = prs >> 8;
  output[23] = prs & 0xff;

  lat = 3541.1493;
  lng = 13945.3994;
  flag = B00000001;
  //  output[24] = byte(lat);
  //  output[28] = byte(lng);
  //  output[32] = byte(flag);
  //  output[24] = lat >> 24;
  //  output[25] = lat >> 16 & 0xff;
  //  output[26] = lat >> 8 & 0xff;
  //  output[27] = lat & 0xff;
  //  output[28] = lng >> 24;
  //  output[29] = lng >> 16 & 0xff;
  //  output[30] = lng >> 8 & 0xff;
  //  output[31] = lng & 0xff;
  output[32] = flag;


  for (int i = 0; i < 33; i++) {
    Serial.print(output[i], BIN);
  }
  //  Serial.println((BIN)accX);
  //  Serial.println(accX,BIN);


}

void loop() {
  // put your main code here, to run repeatedly:

}
