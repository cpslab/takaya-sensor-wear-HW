#include "NineAxesMotion.h"        //Contains the bridge code between the API and the Arduino Environment
#include <Wire.h>
#define INT_PIN 35   // Interrupt用ピン
#define RESET_PIN 32 // Reset用ピン(使わないだろうけど)

#define CALIBRATION_GRAV_TIME 10 // 重力方向のキャリブレーションを行う時間
#define WARNING_THRESHOLD 10 // 警告を出す秒数(デフォルトと重力方向が違うときにカウンタが増える。増えたカウンタがこの値に達したとき警告)

#define GRAV_X 1 // 重力方向を示す定数
#define GRAV_Y 2
#define GRAV_Z 3
String axis[] = {"error","X","Y","Z"}; // print用配列 (上の定数を1~3で作ったため0~2で参照できないので0要素として"error"が入ってる…だれかfixして)

int defaultGravVector = 0; // キャリブレーション時の重力方向(立っている状態での重力方向)
int nowGravVector = 0; // 現在の重力方向
int differentGravCount = 0; // キャリブレーション時と違う方向に重力が向いてたらインクリメントされていく

NineAxesMotion mySensor;   //Object that for the sensor
unsigned long lastStreamTime = 0;     //To store the last streamed time stamp
const int streamPeriod = 2000;
float accX, accY, accZ, magX, magY, magZ, gyoX, gyoY, gyoZ, tmp, hum, prs;
// ↓閾値なんだけど単位がよくわかってない
int threshold = 1;              //At a Range of 4g, the threshold is set at 39.05mg or 0.3830m/s2. This Range is the default for NDOF Mode (この値以上の加速度が与えられると加速度センサが更新されるらしい)
int duration = 1;               //At a filter Bandwidth of 62.5Hz, the duration is 8ms. This Bandwidth is the default for NDOF Mode
int deadCount = 0;  // 死んだ回数を数えます

void setup() //This code is executed once
{
  //Peripheral Initialization
  Serial.begin(115200);           //Initialize the Serial Port to view information on the Serial Monitor
  I2C.begin();                    //Initialize I2C communication to the let the library communicate with the sensor.
  //Sensor Initialization
  Serial.println("Please wait. Initialization in process.");
  initBNO055();
  //init GravityVector
  initGravVector();
}
/*------------------mainLoop--------------------------*/
void loop() //This code is looped forever
{
  if ((millis() - lastStreamTime) >= streamPeriod)
  {
    lastStreamTime = millis();
    updateGravCount();
    printSensorVal();
  }

  if (!isWearerArrived()) {
    Serial.print("Warning!!:"); Serial.println(deadCount); // 確認用カウンタ表示
    deadCount++;  // 死んだ回数を増やします
  }
}
/*-----------------------------------------------------*/

void initBNO055() {
  mySensor.initSensor();          //The I2C Address can be changed here inside this function in the library
  pinMode(INT_PIN, INPUT_PULLUP); // ライブラリ側の設定を無視してINT_PINとRESET_PINを再設定(ライブラリ側でもpinMode()を2つ呼び出してるだけ)
  pinMode(RESET_PIN, OUTPUT); // 同上、今回使わないだろうけど一応。
  // ↓正直よくわかっていない
  mySensor.setOperationMode(OPERATION_MODE_NDOF);   //Can be configured to other operation modes as desired いろいろMODEがある。NDOFは9 Dgrees of Freedom Sensor Fusion
  //mySensor.setOperationMode(OPERATION_MODE_AMG);   //Can be configured to other operation modes as desired
  mySensor.setPowerMode(POWER_MODE_LOWPOWER);  // NORMAL(センサ値読める) or LOWPOWER(センサ値読める) or SUSPEND(センサ値読めない)
  mySensor.setUpdateMode(AUTO);  //The default is AUTO. Changing to manual requires calling the relevant update functions prior to calling the read functions
  mySensor.writeAccelConfig(ACCEL_RANGE_16G, ACCEL_BW_1000HZ, ACCEL_LOWPOWER_1); //色々設定できるよ(ACCCEL  RANGE:2,4,8,16G   BW:7.81,15.63,31.25,62.5,125,250,500,1000HZ   mode:ACCEL_NORMAL, LOWPOWER_1,LOWPOWER_2, SUSPENDとか)
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

void initGravVector() {
  int setupCount = 0;
  int beforeGrav = 0;
  while (setupCount < CALIBRATION_GRAV_TIME) {
    if ((millis() - lastStreamTime) >= streamPeriod) {
      lastStreamTime = millis();
      nowGravVector = checkGrav();
      if (beforeGrav != nowGravVector) {
        beforeGrav = nowGravVector;
        Serial.println();
        Serial.print("nowGravity:"); Serial.println(axis[nowGravVector]);
        setupCount = 0;
      } else {
        Serial.print(".");
        defaultGravVector = nowGravVector;
        setupCount++;
      }
      beforeGrav = nowGravVector;
    }
  }
  Serial.println();
  Serial.print("initGrav_end:"); Serial.print("defaultGravity="); Serial.println(axis[defaultGravVector]);
}

int checkGrav() { // 現在の重力方向を出力する関数
  int gravVector;
  float gravX = mySensor.readGravAccelX();
  float gravY = mySensor.readGravAccelY();
  float gravZ = mySensor.readGravAccelZ();

  gravX = abs(gravX); gravY = abs(gravY); gravZ = abs(gravZ); // 絶対値に変換(比較を行うため)
  
  if (gravX < gravY) {
    if (gravY < gravZ) {
      gravVector = GRAV_Z;
    } else {
      gravVector = GRAV_Y;
    }
  } else {
    if (gravX < gravZ) {
      gravVector = GRAV_Z;
    } else {
      gravVector = GRAV_X;
    }
  }
  return gravVector; // 1:X 2:Y 3:Z
}

void updateGravCount() { // デフォルトと重力方向が異なる場合にカウンタを更新する関数
  nowGravVector = checkGrav();
  if (nowGravVector != defaultGravVector)differentGravCount++;
  else differentGravCount = 0;
}

boolean isWearerArrived() { // 装着者が生きているかどうか判定
  if (differentGravCount >= WARNING_THRESHOLD)return false;
  return true;
}

void printSensorVal() {
  Serial.print("AccRange: ");
  Serial.print(mySensor.readAccelRange(), HEX);
  Serial.println("ms ");
  Serial.print("AccBandWidth: ");
  Serial.print(mySensor. readAccelBandwidth(), HEX);
  Serial.println("ms ");
  Serial.print("Time: ");
  Serial.print(lastStreamTime);
  Serial.println("ms ");
  accX =  mySensor.readAccelerometer(X_AXIS);
  accX =  mySensor.readAccelX();
  Serial.print(" AX: ");
  Serial.println(accX); //Heading data
  //    accY =  (int16_t)mySensor.readAccelerometer(Y_AXIS) * 100;
  accY = mySensor.readAccelerometer(Y_AXIS);
  Serial.print(" AY: ");
  Serial.println(accY); //Roll data
  //    accZ =  (int16_t)mySensor.readAccelerometer(Z_AXIS) * 100;
  accZ = mySensor.readAccelerometer(Z_AXIS);
  Serial.print(" AZ: ");
  Serial.println(accZ); //Pitch data
  //    magX =  (int16_t)mySensor.readMagnetometer(X_AXIS) * 100;
  magX = mySensor.readMagnetometer(X_AXIS);
  Serial.print(" MX: ");
  Serial.println(magX); //Pitch data
  //    magY =  (int16_t)mySensor.readMagnetometer(Y_AXIS) * 100;
  magY = mySensor.readMagnetometer(Y_AXIS);
  Serial.print(" MY: ");
  Serial.println(magY); //Pitch data
  //    magZ =  (int16_t)mySensor.readMagnetometer(Z_AXIS) * 100;
  magZ = mySensor.readMagnetometer(Z_AXIS);
  Serial.print(" MZ: ");
  Serial.println(magZ); //Pitch data
  //    gyoX =  (int16_t)mySensor.readGyro(X_AXIS) * 100;
  gyoX = mySensor.readGyro(X_AXIS);
  Serial.print(" GX: ");
  Serial.println(gyoX); //Pitch data
  //    gyoY =  (int16_t)mySensor.readGyro(Y_AXIS) * 100;
  gyoY = mySensor.readGyro(Y_AXIS);
  Serial.print(" GY: ");
  Serial.println(gyoY); //Pitch data
  //    gyoZ =  (int16_t)mySensor.readGyro(Z_AXIS) * 100;
  gyoZ = mySensor.readGyro(Z_AXIS);
  Serial.print(" GZ: ");
  Serial.println(gyoZ); //Pitch data
  Serial.println("-------------------------------------------");

  Serial.print("differentGravCount:"); Serial.println(differentGravCount);
  Serial.println();
}

