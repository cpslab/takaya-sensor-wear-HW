
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

void measureBNO055() {
  accX =  mySensor.readAccelerometer(X_AXIS);
  accY = mySensor.readAccelerometer(Y_AXIS);
  accZ = mySensor.readAccelerometer(Z_AXIS);
  magX = mySensor.readMagnetometer(X_AXIS);
  magY = mySensor.readMagnetometer(Y_AXIS);
  magZ = mySensor.readMagnetometer(Z_AXIS);
  gyoX = mySensor.readGyro(X_AXIS);
  gyoY = mySensor.readGyro(Y_AXIS);
  gyoZ = mySensor.readGyro(Z_AXIS);
}

