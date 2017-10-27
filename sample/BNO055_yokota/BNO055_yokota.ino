#include "NineAxesMotion.h"        //Contains the bridge code between the API and the Arduino Environment
#include <Wire.h>

#define INT_PIN 13   // Interrupt用ピン
#define RESET_PIN 4 // Reset用ピン(使わないだろうけど)

NineAxesMotion mySensor;   //Object that for the sensor

// ↓閾値なんだけど単位がよくわかってない
int threshold = 5;              //At a Range of 4g, the threshold is set at 39.05mg or 0.3830m/s2. This Range is the default for NDOF Mode
int duration = 1;               //At a filter Bandwidth of 62.5Hz, the duration is 8ms. This Bandwidth is the default for NDOF Mode

int deadCount = 0;  // 死んだ回数を数えます

void setup() //This code is executed once
{
  //Peripheral Initialization
  Serial.begin(115200);           //Initialize the Serial Port to view information on the Serial Monitor
  I2C.begin();                    //Initialize I2C communication to the let the library communicate with the sensor.
  //Sensor Initialization
  Serial.println("Please wait. Initialization in process.");

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
  //digitalPin監視
  //Serial.print("pin D2:");
  //int intPin = digitalRead(2);
  //Serial.println(intPin);

  //InterruptPinの設定はライブラリに依存(元がArduinoのシールド製品用のライブラリのため)
  if (digitalRead(INT_PIN)) { // InterruptPinがHIGHになったとき(=Interruptされたとき)反応する
    Serial.print("I'm dead!!!!!!!!!!!!!!");      Serial.println(deadCount); // 確認用カウンタ表示
    deadCount++;  // 死んだ回数を増やします
    mySensor.resetInterrupt();          //Reset the interrupt line  →これが無いとInterruptのピンがLowに戻らない
    delay(100); // 別にいらないんだけど、これが無いとカウンタが一気に4つくらい増えたりするのでとりま記述
  }
}
