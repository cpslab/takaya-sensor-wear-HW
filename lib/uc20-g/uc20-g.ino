#include "UC20G.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(5, 4); // RX, TX
UC20G myUC20G(&mySerial);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("");
  Serial.println("");

//  bool sora = myUC20G.init("SORACOM.IO", "sora", "sora");
//  Serial.println(sora);
//  delay(20000);
//  sora = myUC20G.disconnect();
//  Serial.println(sora);
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
  }
}
