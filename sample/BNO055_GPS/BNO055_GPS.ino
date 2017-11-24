#include "NineAxesMotion.h"        //Contains the bridge code between the API and the Arduino Environment
#include <Wire.h>

#include <string.h>
#include <ctype.h>
int byteGPS = -1;
char linea[300] = "";
char comandoGPR[7] = "$GPRMC";
int cont = 0;
int bien = 0;
int conta = 0;
int indices[13];

HardwareSerial mySerial(2); //RX,TX

#define INT_PIN 25   // Interrupt用ピン
#define RESET_PIN 12 // Reset用ピン(使わないだろうけど)

NineAxesMotion mySensor;   //Object that for the sensor
unsigned long lastStreamTime = 0;     //To store the last streamed time stamp
const int streamPeriod = 2000;

// ↓閾値なんだけど単位がよくわかってない
int threshold = 5;              //At a Range of 4g, the threshold is set at 39.05mg or 0.3830m/s2. This Range is the default for NDOF Mode
int duration = 1;               //At a filter Bandwidth of 62.5Hz, the duration is 8ms. This Bandwidth is the default for NDOF Mode

int deadCount = 0;  // 死んだ回数を数えます

void setup() //This code is executed once
{
  //Peripheral Initialization
  Serial.begin(9600);           //Initialize the Serial Port to view information on the Serial Monitor
  I2C.begin();                    //Initialize I2C communication to the let the library communicate with the sensor.
  mySerial.begin(9600, SERIAL_8N1, 26, 27);
  //Sensor Initialization
  Serial.println("Please wait. Initialization in process.");
  for (int i = 0; i < 300; i++) { // Initialize a buffer for received data
    linea[i] = ' ';
  }

  mySensor.initSensor();          //The I2C Address can be changed here inside this function in the library
  pinMode(INT_PIN, INPUT_PULLUP); // ライブラリ側の設定を無視してINT_PINとRESET_PINを再設定(ライブラリ側でもpinMode()を2つ呼び出してるだけ)
  pinMode(RESET_PIN, OUTPUT); // 同上、今回使わないだろうけど一応。

  // ↓正直よくわかっていない
  mySensor.setOperationMode(OPERATION_MODE_NDOF);   //Can be configured to other operation modes as desired
  mySensor.setUpdateMode(MANUAL);  //The default is AUTO. Changing to manual requires calling the relevant update functions prior to calling the read functions
  //Setting to MANUAL  requires lesser reads to the sensor

  mySensor.resetInterrupt();  // Interruptピンをリセット

  /*------ここがたぶん重要------*/
  // 以下4つの関数を組み合わせてInterruptしたりしなかったりのタイミングを設定できる様子(ライブラリの読み込みが甘いので正確じゃないかもしれないけど)
  // 一応全パターン動作確認済(2「動いたとき」＋4「動かなくなった時」で両方ともInterruptするとか)
  mySensor.disableAnyMotion(); // 1「動いたとき」はInterruptしないようにする
  //mySensor.enableAnyMotion(threshold, duration, NO_MOTION); // 2「動いたとき(threshold値以上？(未確認)のとき)」はInterruptするようにする
  //mySensor.disableSlowNoMotion(); // 3 「動いていないとき」はIterruptしないようにする
  mySensor.enableSlowNoMotion(threshold, duration, NO_MOTION); // 4「動いていないとき(threshold値以下のとき)」はIterruptするようにする
  /*----------------------------*/

  //↓わからん。監視する軸設定？
  mySensor.accelInterrupts(ENABLE, ENABLE, ENABLE);  //Accelerometer interrupts can be triggered from all 3 axes
}

void loop() //This code is looped forever
{
  if ((millis() - lastStreamTime) >= streamPeriod)
  {
    lastStreamTime = millis();
    mySensor.updateEuler();        //Update the Euler data into the structure of the object
    mySensor.updateCalibStatus();  //Update the Calibration Status

    Serial.print("Time: ");
    Serial.print(lastStreamTime);
    Serial.print("ms ");

    Serial.print(" H: ");
    Serial.print(mySensor.readEulerHeading()); //Heading data
    Serial.print("deg ");

    Serial.print(" R: ");
    Serial.print(mySensor.readEulerRoll()); //Roll data
    Serial.print("deg");

    Serial.print(" P: ");
    Serial.print(mySensor.readEulerPitch()); //Pitch data
    Serial.print("deg ");

    Serial.println();
  }


  byteGPS = mySerial.read();       // Read a byte of the serial port
  if (byteGPS == -1) {           // See if the port is empty yet
    delay(100);
  } else {
    // note: there is a potential buffer overflow here!
    linea[conta] = byteGPS;      // If there is serial port data, it is put in the buffer
    conta++;
    //     Serial.print(byteGPS, BYTE);
    if (byteGPS == 13) {         // If the received byte is = to 13, end of transmission
      // note: the actual end of transmission is <CR><LF> (i.e. 0x13 0x10)
      cont = 0;
      bien = 0;
      // The following for loop starts at 1, because this code is clowny and the first byte is the <LF> (0x10) from the previous transmission.
      for (int i = 1; i < 7; i++) { // Verifies if the received command starts with $GPR
        if (linea[i] == comandoGPR[i - 1]) {
          bien++;
        }
      }
      if (bien == 6) {           // If yes, continue and process the data
        for (int i = 0; i < 300; i++) {
          if (linea[i] == ',') { // check for the position of the  "," separator
            // note: again, there is a potential buffer overflow here!
            indices[cont] = i;
            cont++;
          }
          if (linea[i] == '*') { // ... and the "*"
            indices[12] = i;
            cont++;
          }
        }
        Serial.println("");      // ... and write to the serial port
        Serial.println("");
        Serial.println("---------------");
        for (int i = 0; i < 12; i++) {
          switch (i) {
            case 1 : Serial.print("Status (A=OK,V=KO): "); break;
            case 2 : Serial.print("Latitude: "); break;
            case 3 : Serial.print("Direction (N/S): "); break;
            case 4 : Serial.print("Longitude: "); break;
            case 5 : Serial.print("Direction (E/W): "); break;
            case 12 : Serial.print("Checksum: "); break;
          }
          for (int j = indices[i]; j < (indices[i + 1] - 1); j++) {
            Serial.print(linea[j + 1]);
          }
          Serial.println("");
        }
        Serial.println("---------------");
      }
      conta = 0;                  // Reset the buffer
      for (int i = 0; i < 300; i++) { //
        linea[i] = ' ';
      }
    }
  }


  //InterruptPinの設定はライブラリに依存(元がArduinoのシールド製品用のライブラリのため)
  if (digitalRead(INT_PIN)) { // InterruptPinがHIGHになったとき(=Interruptされたとき)反応する
    //    Serial.print("I'm dead!!!!!!!!!!!!!!");      Serial.println(deadCount); // 確認用カウンタ表示
    Serial.print("Moved!:");      Serial.println(deadCount); // 確認用カウンタ表示
    deadCount++;  // 死んだ回数を増やします
    mySensor.resetInterrupt();          //Reset the interrupt line  →これが無いとInterruptのピンがLowに戻らない
    //    delay(100); // 別にいらないんだけど、これが無いとカウンタが一気に4つくらい増えたりするのでとりま記述
  }
}
