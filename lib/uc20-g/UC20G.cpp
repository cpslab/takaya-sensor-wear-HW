#include <SoftwareSerial.h>
#include "UC20G.h
"
//constractor
UC20G::UC20G(int rx_pin, int tx_pin)
{
  Rx_Pin = rx_pin;
  Tx_Pin = tx_pin;
  SoftwareSerial mySerial(Rx_Pin,Tx_Pin);
  mySerial.begin(115200);
}

boolean UC20G::UC20G_init(String apn, String user, String pass)
{
  //AT+QICSGP=1,1,"SORACOM.IO","sora","sora",0
  String conn = "AT+QICSGP=1,1,\"" + apn + "\",\"" + user + "\",\"" + pass;
  mySerial.println(conn);
    
//  String ret = mySerial.readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
//  if(ret.indexOf("OK")){    //ちゃんと動くか未確認
//    return true;
//  }else{
//    return false;
//  }
  return true;  //今はてきとうに返してる モジュールからの戻り値をちゃんと読んで適切に返す
}

boolean UC20G::UC20G_disconnect()
{
  String disc = "AT+QIDEACT=1"
  mySerial.println(disc);

//  String ret = mySerial.readStringUntil('\n');    //タイムアウトをつけないとここで止まりそう
//  if(ret.indexOf("OK")){    //ちゃんと動くか未確認
//    return true;
//  }else{
//    return false;
//  }
  return true;  //今はてきとうに返してる モジュールからの戻り値をちゃんと読んで適切に返す
}

