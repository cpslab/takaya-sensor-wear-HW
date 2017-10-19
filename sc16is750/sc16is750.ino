#include "Wire.h"

const int addr = (0x98 >> 1);    // define the I2C Address
// general register definition
const uint8_t RHR = (0x00 << 3); //Receive Holding Register
const uint8_t THR = (0x00 << 3); //Transmit Holding Register
const uint8_t FCR = (0x02 << 3); //FIFO Control Register
const uint8_t LCR = (0x03 << 3); //FLine Control Register
const uint8_t MCR = (0x04 << 3); //Modem Control Register
const uint8_t LSR = (0x05 << 3); //Line Status Register
const uint8_t RXLVL = (0x09 << 3); //Receiver FIFO Level register
// special register set
const uint8_t DLL = (0x00 << 3); //Division registers (DLL)
const uint8_t DLH = (0x01 << 3); //Division registers (DLH)
//enhanced register set
const uint8_t EFR = (0x02 << 3);

String dataes[6];
uint8_t j = 0;
bool flag = false;
/*---------------------------------------------------
    Serial
*/
void setup() {            // base CK = 12MHz
  Serial.begin(9600);
  Wire.begin();
  char cmd[2];
  Serial.println("Wire.begin()");
  Wire.beginTransmission(addr);
  // ボーレイト設定
  cmd[0] = LCR;
  cmd[1] = 0x83;
  Wire.write(cmd, 2);    // Divisor Latch Enable
  Wire.endTransmission();

  Wire.beginTransmission(addr);
  cmd[0] = DLL;
  cmd[1] = 78;
  Wire.write(cmd, 2);    // divisor latch LSB
  Wire.endTransmission();

  Wire.beginTransmission(addr);
  cmd[0] = DLH;
  cmd[1] = 0;
  Wire.write(cmd, 2);    // divisor latch MSB
  Wire.endTransmission();

  Wire.beginTransmission(addr);
  // 通信フォーマット設定
  cmd[0] = LCR;
  cmd[1] = 0x03;
  Wire.write(cmd, 2);
  Wire.endTransmission();

  // FIFO設定
  Wire.beginTransmission(addr);
  cmd[0] = FCR;
  cmd[1] = 0xc7;
  Wire.write(cmd, 2);
  Wire.endTransmission();
  Serial.println("end setup");

  for (int i = 0; i < 6; i++) {
    dataes[i] = "";
  }

  //  Serial.println(Wire.available());
}
/*---------------------------------------------------
    Main Routine
*/

void loop() {
  uint8_t i, n, data;
  Wire.beginTransmission(addr);
  Wire.write(LSR);
  Wire.endTransmission();
  Wire.requestFrom(addr, (uint8_t)1);
  data = Wire.read();
  //  Serial.print(data,HEX);
  //  Serial.println();
  if (data == 0x61) { //at least one character in the RX FIFO (Serial.available())
    //    Serial.print("read:");
    Wire.beginTransmission(addr);
    Wire.write(RXLVL);
    Wire.endTransmission();
    Wire.requestFrom(addr, (uint8_t)1);
    n = Wire.read(); //読み込むデータの数
    //    Serial.print("n=");
    //    Serial.println(n);
    Wire.beginTransmission(addr);
    Wire.write(RHR);
    Wire.endTransmission();
    Wire.requestFrom(addr, (uint8_t)n);

    for (i = 0; i < n; i++) {
      //writeValue ( Wire.read() ); //読んだデータをそのままエコーバック
      char Word = (char)Wire.read();
      if (j == 6) {
        for (i = 0; i < 6; i++) {
          dataes[i] = "";
        }
        j = 0;
      }
      if (Word == '$') {
        dataes[j] += Word;
        flag = true;
        continue;
      }
      if (Word == '\n' || Word == '\r') {
        dataes[j] += '\n';
        j++;
        continue;
      }
      if (flag) {
        dataes[j] += Word;
      }
    }
    for (i = 0; i < j; i++) {
      if(dataes[i].startsWith("$GPGGA")){
        Serial.print(dataes[i]);
      }
    }
  }
  delay(1);
}
