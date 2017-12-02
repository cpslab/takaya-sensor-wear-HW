#include "UC20G.h"
#include <SoftwareSerial.h>

int16_t accX, accY, accZ, magX, magY, magZ, gyoX, gyoY, gyoZ, tmp, hum, prs;
float lat, lng;
byte flag;
char output[33]; //24+8+1

SoftwareSerial mySerial(5, 4); // RX, TX
UC20G myUC20G(&mySerial);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("");
  Serial.println("");

}

char input = ' ';
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    input = Serial.read();
  }
  switch(input){
    case 'a':
      myUC20G.init("SORACOM.IO", "sora", "sora");
      break;
    case 'b':
      myUC20G.disconnect();
      break;
    case 'c':
      myUC20G.disable();
      break;
    case 's':
      update_data();
//      myUC20G.udpSend("beam.soracom.io", "23080", "[{hoge:huga}]");
      myUC20G.udpSend("beam.soracom.io", "23080", output);
      break;
    case 't':
      myUC20G.at();
      break;
  }
}


void update_data() {

  accX = 9999;
  accY = 9999;
  accZ = 9999;

  output[0] = accX >> 8;
  output[1] = accX & 0xff;
  output[2] = accY >> 8;
  output[3] = accY & 0xff;
  output[4] = accZ >> 8;
  output[5] = accZ & 0xff;


  magX = 8888;
  magY = 8888;
  magZ = 8888;

  output[6] = magX >> 8;
  output[7] = magX & 0xff;
  output[8] = magY >> 8;
  output[9] = magY & 0xff;
  output[10] = magZ >> 8;
  output[11] = magZ & 0xff;

  gyoX = 7777;
  gyoY = 7777;
  gyoZ = 7777;

  output[12] = gyoX >> 8;
  output[13] = gyoX & 0xff;
  output[14] = gyoY >> 8;
  output[15] = gyoY & 0xff;
  output[16] = gyoZ >> 8;
  output[17] = gyoZ & 0xff;

  tmp = 2222;
  hum = 2222;
  prs = 123456;

  output[18] = tmp >> 8;
  output[19] = tmp & 0xff;
  output[20] = hum >> 8;
  output[21] = hum & 0xff;
  output[22] = prs >> 8;
  output[23] = prs & 0xff;
//
//  lat = 3541.1493;
//  lng = 13945.3994;
  
output[24] = 0xff;
output[25] = 0xff;
output[26] = 0xff;
output[27] = 0xff;

output[28] = 0xff;
output[29] = 0xff;
output[30] = 0xff;
output[31] = 0xff;
  
  flag = B00000001;

  output[32] = flag;


//  for (int i = 0; i < 33; i++) {
//    Serial.print(output[i], BIN);
//  }
  //  Serial.println((BIN)accX);
  //  Serial.println(accX,BIN);


}
