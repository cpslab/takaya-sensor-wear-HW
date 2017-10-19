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


bool UC20G::udpSend(String ipAdress, String port, const char* message)
{
  String str,ret,sendMessage;

  clearSerialBuffer();

  // AT+QIOPEN
  str = "AT+QIOPEN=1,0,\"UDP\",\"" + ipAdress  + "\"," + port +",0,1\r\n";
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret); // AT~
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret); // OK
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret); // 空白
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret); // +QIOPEN: 0,0

  // 文字の抜き出し
  int indexA,indexB;
  String resultA,resultB;
  indexA = ret.indexOf(":");
  indexB = ret.indexOf(",");
  Serial.println(indexA);
  Serial.println(indexB);
  resultA = ret.substring(indexA+2,indexB);
  Serial.println(resultA);
  
  // 文字のカウント
  int len = strlen(message);
  Serial.println(len);
  String l = String(len);

  // AT+QISEND
  str = "AT+QISEND="+resultA+","+l+"\r\n";
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret); // AT+QISEND=0,13
  ret = uc20SwSerial->readStringUntil('>');
  Serial.println(ret); // >
  str = message;
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret); // [{hoge:huga}]
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret); // send ok
  sendMessage = ret;

  // AT+QICLOSE = 1
  str = "AT+QICLOSE=1\r\n";
  uc20SwSerial->print(str);
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret); // AT+QICLOSE=1
  ret = uc20SwSerial->readStringUntil('\n');
  Serial.println(ret); // OK

  if(sendMessage.indexOf("SEND OK") == -1){
    Serial.println("Send Failed");
    return false;
  }else {
    Serial.println("Send Sucsess");
    return true;
  }
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

