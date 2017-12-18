#include "BME280_MOD-1022.h"
#include "NineAxesMotion.h"
#include <Wire.h>
#define INT_PIN 35   // Interrupt用ピン
#define RESET_PIN 5 // Reset用ピン とりあえずでSensor_VNに設定
#define CALIBRATION_GRAV_TIME 1 // 重力方向のキャリブレーションを行う時間
#define WARNING_THRESHOLD 10 // 警告を出す秒数(デフォルトと重力方向が違うときにカウンタが増える。増えたカウンタがこの値に達したとき警告)

#define GRAV_X 1 // 重力方向を示す定数
#define GRAV_Y 2
#define GRAV_Z 3
String axis[] = {"error", "X", "Y", "Z"}; // print用配列 (上の定数を1~3で作ったため0~2で参照できないので0要素として"error"が入ってる…だれかfixして)

int defaultGravVector = 0; // キャリブレーション時の重力方向(立っている状態での重力方向)
int nowGravVector = 0; // 現在の重力方向
int differentGravCount = 0; // キャリブレーション時と違う方向に重力が向いてたらインクリメントされていく

NineAxesMotion mySensor;   //Object that for the sensor
// ↓閾値なんだけど単位がよくわかってない
int threshold = 1;              //At a Range of 4g, the threshold is set at 39.05mg or 0.3830m/s2. This Range is the default for NDOF Mode (この値以上の加速度が与えられると加速度センサが更新されるらしい)
int duration = 1;               //At a filter Bandwidth of 62.5Hz, the duration is 8ms. This Bandwidth is the default for NDOF Mode
int deadCount = 0;  // 死んだ回数を数えます

unsigned long lastStreamTime = 0;     //To store the last streamed time stamp
const int streamPeriod = 1000;        //　間欠動作時間

//float accX, accY, accZ, magX, magY, magZ, gyoX, gyoY, gyoZ, tmp, hum, prs, lat, lng, eX, eY, eZ;
float accX, accY, accZ, magX, magY, magZ, gyoX, gyoY, gyoZ, tmp, hum, prs, lat, lng;
int8_t flag = 0;
String jsonData;  //送信用のデータを整形して保存
//char output[33]; //24+8+1

HardwareSerial  Serial2(1); // UC20Gとの通信ポート

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);           //Initialize the Serial Port to view information on the Serial Monitor
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  I2C.begin();                    //Initialize I2C communication to the let the library communicate with the sensor.
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  //Sensor Initialization
  Serial.println("Please wait. Initialization in process.");
  initBNO055();
  //init GravityVector
  initGravVector();
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - lastStreamTime) >= streamPeriod)
  {
    lastStreamTime = millis();
    updateGravCount();
    measureBNO055();
    measureBME280();
    makeSendData();
  }

  if (!isWearerArrived()) {
    //    Serial.print("Warning!!:"); Serial.println(deadCount); // 確認用カウンタ表示
    //    deadCount++;  // 死んだ回数を増やします
    flag = 1;
  } else {
    flag = 0;

  }
}

void makeSendData() {
  jsonData = "";
  jsonData = "{ \"accX\": ";    jsonData.concat(accX);
  jsonData.concat(", \"accY\": ");    jsonData.concat(accY);
  jsonData.concat(", \"accZ\": ");    jsonData.concat(accZ);
  jsonData.concat(", \"magX\": ");    jsonData.concat(magX);
  jsonData.concat(", \"magY\": ");    jsonData.concat(magY);
  jsonData.concat(", \"magZ\": ");    jsonData.concat(magZ);
  jsonData.concat(", \"gyroX\": ");    jsonData.concat(gyoX);
  jsonData.concat(", \"gyroY\": ");    jsonData.concat(gyoY);
  jsonData.concat(", \"gyroZ\": ");    jsonData.concat(gyoZ);
  //    jsonData.concat(", \"angleP\": ");    jsonData.concat(eX);
  //    jsonData.concat(", \"angleR\": ");    jsonData.concat(eY);
  //    jsonData.concat(", \"angleY\": ");    jsonData.concat(eZ);
  jsonData.concat(", \"temp\": ");    jsonData.concat(tmp);
  jsonData.concat(", \"humid\": ");    jsonData.concat(hum);
  jsonData.concat(", \"airPressure\": ");    jsonData.concat(prs);
  jsonData.concat(", \"lat\": ");    jsonData.concat(lat);
  jsonData.concat(", \"lng\": ");    jsonData.concat(lng);
  jsonData.concat(", \"flags\": ");    jsonData.concat(flag);
  Serial.print("json:");
  Serial.println(jsonData);
}

