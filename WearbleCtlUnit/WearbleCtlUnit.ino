#include "BME280_MOD-1022.h"
#include "NineAxesMotion.h"
#include <Adafruit_NeoPixel.h>
#include "esp32-hal-log.h"
#include "freertos/task.h"
#include <Wire.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define CALIBRATION_GRAV_TIME 1 // 重力方向のキャリブレーションを行う時間
#define WARNING_THRESHOLD 10 // 警告を出す秒数(デフォルトと重力方向が違うときにカウンタが増える。増えたカウンタがこの値に達したとき警告)

#define GRAV_X 1 // 重力方向を示す定数
#define GRAV_Y 2
#define GRAV_Z 3

// ピンアサイン
#define LED 27 //右側
#define VIBRATOR1 14 //右側
#define VIBRATOR2 13 //左側
#define PIXELS 6
#define NEOPIXEL1 26
#define NEOPIXEL2 25
#define INT_PIN 35   // BNO055 Interrupt用ピン
#define RESET_PIN 37 // BNO055 Reset用ピン とりあえずでSensor_VNに設定
#define TXD1 33
#define RXD1 32
#define TXD2 17
#define RXD2 16
#define W_DISABLE 12
#define PERST 15
#define SW1   0

#define GPS_ADDR 0x90

String axis[] = {"error", "X", "Y", "Z"}; // print用配列 (上の定数を1~3で作ったため0~2で参照できないので0要素として"error"が入ってる…だれかfixして)

uint8_t defaultGravVector = 0; // キャリブレーション時の重力方向(立っている状態での重力方向)
uint8_t nowGravVector = 0; // 現在の重力方向
uint8_t differentGravCount = 0; // キャリブレーション時と違う方向に重力が向いてたらインクリメントされていく

// ↓閾値なんだけど単位がよくわかってない
uint8_t threshold = 1;              //At a Range of 4g, the threshold is set at 39.05mg or 0.3830m/s2. This Range is the default for NDOF Mode (この値以上の加速度が与えられると加速度センサが更新されるらしい)
uint8_t duration = 1;               //At a filter Bandwidth of 62.5Hz, the duration is 8ms. This Bandwidth is the default for NDOF Mode
//uint8_t deadCount = 0;  // 死んだ回数を数えます

unsigned long lastStreamTime = 0;     //To store the last streamed time stamp
const int streamPeriod = 1000;        //　センサデータの更新頻度
unsigned long lastStreamTimeGPS = 0;     //To store the last streamed time stamp
const int streamPeriodGPS = 20;//←ここの数値を30以上に上げると正常にGPS値取れない
unsigned long lastStreamTimeSIM = 0;
const int streamPeriodSIM = 6000;    //データのアップロード頻度
//const int streamPeriodSIM = 60000 * 5;    //データのアップロード頻度

//float accX, accY, accZ, magX, magY, magZ, gyoX, gyoY, gyoZ, tmp, hum, prs, lat, lng, eX, eY, eZ;
float accX, accY, accZ, magX, magY, magZ, gyoX, gyoY, gyoZ, tmp, hum, prs, lat, lng;
uint8_t sensorFlag = 0;
String jsonData;  //送信用のデータを整形して保存

boolean vib = false;  //振動用タスクのフラグ
boolean led = false;  //neopixel用タスクのフラグ

boolean ota = false;  //OTA起動のフラグ
const char* ssid = "NNNEXUS5X";
const char* password = "nnkpoint";



typedef struct {
  float La;
  String latitude;
  float Lo;
  String longitude;
} gpsdata;


HardwareSerial  Serial1(1); // TweLiteとの通信ポート
HardwareSerial  Serial2(2); // UC20Gとの通信ポート
NineAxesMotion mySensor;   //Object that for the sensor
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(6, NEOPIXEL1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(6, NEOPIXEL2, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);           //Initialize the Serial Port to view information on the Serial Monitor
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(SW1, INPUT);           //W_DISABLE PIN
  initVibration();
  initNeopixel();

  xTaskCreatePinnedToCore(vib_t, "vib_t", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(led_t, "led_t", 4096, NULL, 1, NULL, 0);

  delay(1000);
  if (digitalRead(SW1) == 0) {
    ota = true;
    neopixelColor(0, 0, 255);
    otaSetup();
  }
  else {
    ota = false;
    normalSetup();
  }
}


void loop() {
  if (ota) {
    ArduinoOTA.handle();
  }
  else {
    if ((millis() - lastStreamTime) >= streamPeriod)
    {
      lastStreamTime = millis();
      updateGravCount();
      measureBNO055();
      measureBME280();
      makeSendData();
      //    sendDataUDP(jsonData);
    }

    if (!isWearerArrived()) {
      sensorFlag = 1;
      led = true;
    } else {
      sensorFlag = 0;
    }

    if ((millis() - lastStreamTimeSIM) >= streamPeriodSIM)
    {
      lastStreamTimeSIM = millis();
      sendDataUDP(jsonData);
    }


    if ((millis() - lastStreamTimeGPS) >= streamPeriodGPS)
    {
      lastStreamTimeGPS = millis();
      measureGPS(getGPS());
    }

    readTweLite();
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
  //  jsonData.concat(", \"angleP\": ");    jsonData.concat(eX);
  //  jsonData.concat(", \"angleR\": ");    jsonData.concat(eY);
  //  jsonData.concat(", \"angleY\": ");    jsonData.concat(eZ);
  jsonData.concat(", \"temp\": ");    jsonData.concat(tmp);
  jsonData.concat(", \"humid\": ");    jsonData.concat(hum);
  jsonData.concat(", \"airPressure\": ");    jsonData.concat(prs);
  jsonData.concat(", \"lat\": ");    jsonData.concat(lat);
  jsonData.concat(", \"lng\": ");    jsonData.concat(lng);
  jsonData.concat(", \"flags\": ");    jsonData.concat(sensorFlag);
  jsonData.concat("}");
  Serial.print("json:");
  Serial.println(jsonData);
}

void sendDataUDP(String str) {
  char ud[str.length() + 1];
  str.toCharArray(ud, str.length() + 1);

  Serial.print("ud:");
  Serial.println(ud);
  udpSend("beam.soracom.io", "23080", ud);
}


/////////////////////////////////////////
//            Setup
///////////////////////////////////////
void otaSetup() {
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void normalSetup() {
  //    pinMode(W_DISABLE, OUTPUT);           //W_DISABLE PIN
  //    digitalWrite(W_DISABLE, LOW);
  //      pinMode(PERST, OUTPUT);           //PERST PIN
  //    digitalWrite(PERST, LOW);

  //Sensor Initialization
  Serial.println("Please wait. Initialization in process.");
  colorWipe(strip1.Color(255, 255, 0), 100);
  //  neopixelOff();
  I2C.begin();                    //Initialize I2C communication to the let the library communicate with the sensor.
  initGPS();
  initBNO055();
  initGravVector();
  initUC20("SORACOM.IO", "sora", "sora");
  neopixelOff();

}

