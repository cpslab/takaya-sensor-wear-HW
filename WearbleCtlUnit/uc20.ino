bool initUC20(String apn, String user, String pass)
{
  //最初にとりあえずデアクティベートしておくことでアクティベート時のエラー回避
  String str, ret;

//  clearSerialBuffer();
//  str = "AT+QIDEACT=1\r\n";
//  Serial2.print(str);
//  ret = Serial2.readStringUntil('\n');
//  Serial.println(ret);
//  ret = Serial2.readStringUntil('\n');
//  Serial.println(ret);
//  if (ret.indexOf("OK") == -1) {  //ちゃんと動くか未確認
//    Serial.println("miss init deact");
//    return false;
//  }

  clearSerialBuffer();
  str = "AT+QICSGP=1,1,\"" + apn + "\",\"" + user + "\",\"" + pass + "\",0\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');  //打ったコマンドが帰ってくる
  Serial.println(ret);
  ret = Serial2.readStringUntil('\n');  //コマンドに対する応答
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {  //ちゃんと動くか未確認
    Serial.println("miss csgp");
    return false;
  }

  clearSerialBuffer();
  str = "AT+QIACT=1\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');  //打ったコマンドが帰ってくる
  Serial.println(ret);
  ret = Serial2.readStringUntil('\n');  //コマンドに対する応答
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {  //ちゃんと動くか未確認
    Serial.println("miss act");
    return false;
  }

  return true;
}

int split(String data, char delimiter, String *dst){
    int index = 0;
    int arraySize = (sizeof(data)/sizeof((data)[0]));  
    int datalength = data.length();
    for (int i = 0; i < datalength; i++) {
        char tmp = data.charAt(i);
        if ( tmp == delimiter ) {
            index++;
            if ( index > (arraySize - 1)) return -1;
        }
        else dst[index] += tmp;
    }
    return (index + 1);
}

bool disconnectUC20()
{
  String str, ret;

  clearSerialBuffer();
  str = "AT+QIDEACT=1\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');  //打ったコマンドが帰ってくる
  Serial.println(ret);
  ret = Serial2.readStringUntil('\n');  //コマンドに対する応答
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {
    Serial.println("miss deact");
    return false;
  }

  return true;
}

bool disableUC20()
{
  String str, ret;

  clearSerialBuffer();
  str = "AT+QPOWD\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');  //打ったコマンドが帰ってくる
  Serial.println(ret);
  ret = Serial2.readStringUntil('\n');  //コマンドに対する応答
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {
    Serial.println("miss deact");
    return false;
  }

  return true;
}


void powerOnUC20() {
  digitalWrite(12, HIGH);
  delay(1000);
  digitalWrite(12, LOW);
}


bool udpSend(String ipAdress, String port, const char* message)
{
  String str, ret, sendMessage;

  clearSerialBuffer();

  // AT+QIOPEN
  str = "AT+QIOPEN=1,0,\"UDP\",\"" + ipAdress  + "\"," + port + ",0,1\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret); // AT~
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret); // OK
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret); // 空白
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret); // +QIOPEN: 0,0

  // 文字の抜き出し
  int indexA, indexB;
  String resultA, resultB;
  indexA = ret.indexOf(":");
  indexB = ret.indexOf(",");
  Serial.println(indexA);
  Serial.println(indexB);
  resultA = ret.substring(indexA + 2, indexB);
  Serial.println(resultA);

  // 文字のカウント
  int len = strlen(message);
  Serial.println(len);
  String l = String(len);

  // AT+QISEND
  str = "AT+QISEND=" + resultA + "," + l + "\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret); // AT+QISEND=0,13
  ret = Serial2.readStringUntil('>');
  Serial.println(ret); // >
  str = message;
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret); // [{hoge:huga}]
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret); // send ok
  sendMessage = ret;

  // AT+QICLOSE = 1
  str = "AT+QICLOSE=1\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret); // AT+QICLOSE=1
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret); // OK

  if (sendMessage.indexOf("SEND OK") == -1) {
    Serial.println("Send Failed");
    return false;
  } else {
    Serial.println("Send Sucsess");
    return true;
  }
}


bool at() {
  String str, ret;

  clearSerialBuffer();
  str = "AT\r\n";
  Serial.print(str);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret);
  if (ret.indexOf("OK") == -1) {
    Serial.println("miss deact");
    return false;
  }
  Serial.println("at program");
  return true;
}

void clearSerialBuffer() {
  while (1) {
    if (Serial2.available()) {
      Serial2.read();
    } else {
      break;
    }
  }
}

bool getImei() {
  String str, ret;

  clearSerialBuffer();
  str = "AT+CIMI\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret);  //オウム返し
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret);  //IMEI
  imei = ret;
  ret = Serial2.readStringUntil('\n');
  ret = Serial2.readStringUntil('\n');
  if (ret.indexOf("OK") == -1) {
    Serial.println("miss get imei");
    return false;
  }
  Serial.println("sucess get imei");
  return true;
}

bool getWeb(const char* url) {
  String str, ret,u;

  u = "";
  u.concat(url);
  u.concat(imei);
  
  int len = u.length();
  Serial.println(len);
  String l = String(len);

  clearSerialBuffer();
  str = "AT+QHTTPURL="+ l +"\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret);
  if (ret.indexOf("CONNECT") == -1) {
    Serial.println("miss connect QHTTPURL");
    return false;
  }
  str=u+"\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');
  Serial.println("url1:"+ret);
  ret = Serial2.readStringUntil('\n');
  Serial.println("url2:"+ret);
  if (ret.indexOf("OK") == -1) {
    Serial.println("miss url");
    return false;
  }
  str = "AT+QHTTPGET\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');
  Serial.println("get1:"+ret);
  ret = Serial2.readStringUntil('\n');
  Serial.println("get2:"+ret);
  if (ret.indexOf("OK") == -1) {
    Serial.println("miss QHTTPGET");
    return false;
  }
  ret = Serial2.readStringUntil('\n');
  Serial.println("get3:"+ret);
  ret = Serial2.readStringUntil('\n');
  Serial.println("get4:"+ret);
  ret = Serial2.readStringUntil('\n');
  Serial.println("get5:"+ret);
  str = "AT+QHTTPREAD\r\n";
  Serial2.print(str);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret);
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret);
  if (ret.indexOf("CONNECT") == -1) {
    Serial.println("miss connect QHTTPREAD");
    return false;
  }
  ret = Serial2.readStringUntil('\n');
  Serial.println(ret);

  String messages[7];

  int index = split(ret,',',messages);
  if(index != 7){
    Serial.println("fails get message");
  }

  webHH = messages[0].toInt();
  webmm = messages[1].toInt();
  webss = messages[2].toInt();
  ledpatt = messages[3].toInt();
  vibpatt = messages[4].toInt();
  freq_poll = messages[5].toInt();
  ferq_upload = messages[6].toInt();
  
  return true;
}
