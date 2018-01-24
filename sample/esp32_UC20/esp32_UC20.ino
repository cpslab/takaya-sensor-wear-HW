HardwareSerial Serial1(1); // RX, TX
void setup()
{
//  pinMode(12, OUTPUT);
//  digitalWrite(12, LOW);
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17); //UC20
  //  Serial1.begin(115200, SERIAL_8N1, 32, 33); //Twe-Lite

}
void loop()
{
  if (Serial1.available())
    Serial.write(Serial1.read());
  if (Serial.available()) {
    char c = Serial.read();
    Serial1.write(c);
  }
}

void enable() {
  digitalWrite(12, HIGH);
  delay(1000);
  digitalWrite(12, LOW);
}


/*
 * AT+QICSGP=1,1,"SORACOM.IO","sora","sora",0   
 * AT+CIMIでIMEIを取得
 * AT+QIACT=1でオンライン
 * AT+QIDEACT=1でオフライン
 * 
 * GETの流れ
 * AT+QHTTPURL=<url length>
 * →:CONNECT
 * →URL入力
 * →：
 * →:OK
 * AT+QHTTPGET
 * →:OK
 * →：
 * →：+QHTTPGET: 0,200,1199
 * AT+QHTTPREAD
 * →：CONNECT
 * →：
 * →<GET parameter>
 */
