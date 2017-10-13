#include <SoftwareSerial.h>
#include "UC20G.h"
#include <Arduino.h>

#define DEBUG 1
//constractor
UC20G::UC20G(SoftwareSerial *ser)
{
  uc20SwSerial = ser;
  uc20SwSerial->begin(115200);
  uc20SwSerial->setTimeout(10000);
}

bool UC20G::init(String apn, String user, String pass)
{
  //最初にとりあえずデアクティベートしておくことでアクティベート時のエラー回避
  String str, ret;

  clearSerialBuffer();
  str = "AT+QIDEACT=1\r\n";
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {  //ちゃんと動くか未確認
    Serial.println("miss init deact");
    return false;
  }

  clearSerialBuffer();
  str = "AT+QICSGP=1,1,\"" + apn + "\",\"" + user + "\",\"" + pass + "\",0\r\n";
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');  //打ったコマンドが帰ってくる
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');  //コマンドに対する応答
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {  //ちゃんと動くか未確認
    Serial.println("miss csgp");
    return false;
  }

  clearSerialBuffer();
  str = "AT+QIACT=1\r\n";
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');  //打ったコマンドが帰ってくる
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');  //コマンドに対する応答
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {  //ちゃんと動くか未確認
    Serial.println("miss act");
    return false;
  }


  return true;
}

bool UC20G::disconnect()
{
  String str, ret;

  clearSerialBuffer();
  str = "AT+QIDEACT=1\r\n";
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');  //打ったコマンドが帰ってくる
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');  //コマンドに対する応答
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {
    Serial.println("miss deact");
    return false;
  }

  return true;
}

bool UC20G::disable()
{
  String str, ret;

  clearSerialBuffer();
  str = "AT+QPOWD\r\n";
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');  //打ったコマンドが帰ってくる
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');  //コマンドに対する応答
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {
    Serial.println("miss deact");
    return false;
  }

  return true;
}

bool UC20G::at()
{
  String str,ret;

  clearSerialBuffer();
  str = "AT\r\n";
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {
    Serial.println("miss deact");
    return false;
  }
  Serial.println("at program");
  return true;
}

void UC20G::clearSerialBuffer() {
  while (1) {
    if (uc20SwSerial->available()) {
      uc20SwSerial->read();
    }else{
      break;
    }
  }
}

