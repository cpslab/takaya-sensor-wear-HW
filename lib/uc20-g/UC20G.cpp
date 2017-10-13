#include <SoftwareSerial.h>
#include "UC20G.h"
 #include <Arduino.h> 
//constractor
UC20G::UC20G(SoftwareSerial *ser)
{
  uc20SwSerial = ser;
  uc20SwSerial->begin(115200);
}

bool UC20G::init(String apn, String user, String pass)
{
  //AT+QICSGP=1,1,"SORACOM.IO","sora","sora",0
  String str = "AT+QIDEACT=1\r\n";
  uc20SwSerial->print(str);
  String  ret = uc20SwSerial->readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
  Serial.println(ret);

  str = "AT+QICSGP=1,1,\"" + apn + "\",\"" + user + "\",\"" + pass + "\",0\r\n";
//  String str = "AT+QICSGP=1,1,\"SORACOM.IO\",\"sora\",\"sora\",0\r\n";
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
  Serial.println(ret);
  
  str = "AT+QIACT=1\r\n";
  uc20SwSerial->print(str);
  delay(4000);  
  ret = uc20SwSerial->readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
  Serial.println(ret);
  ret = uc20SwSerial->readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
  Serial.println(ret);
//  if(ret.indexOf("OK")){    //ちゃんと動くか未確認
//    Serial.println("initOK");
//    return true;
//  }else{
//    return false;
//  }
  return true;  //今はてきとうに返してる モジュールからの戻り値をちゃんと読んで適切に返す
}

bool UC20G::disconnect()
{
  String str = "AT+QIDEACT=1";
  uc20SwSerial->println(str);
  delay(4000);  
  String ret = uc20SwSerial->readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
  Serial.println(ret);
  if(ret.indexOf("OK")){    //ちゃんと動くか未確認
    Serial.println("disconnect OK");
    return true;
  }else{
    return false;
  }
//  return true;  //今はてきとうに返してる モジュールからの戻り値をちゃんと読んで適切に返す
}

