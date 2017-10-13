#ifndef UC20G_h
#define UC20G_h
#include <SoftwareSerial.h>

class UC20G
{
  public:
    UC20G(int rx_pin, int tx_pin);
    
    //初期化用の関数
    //AT+QICSGP=1,1,"SORACOM.IO","sora","sora",0
    //AT+QIDEACT=1
    boolean UC20G_init(String apn, String user, String pass);
    boolean UC20G_disconnect();
  private:
    int Rx_Pin;
    int Tx_Pin;
}

#endif
