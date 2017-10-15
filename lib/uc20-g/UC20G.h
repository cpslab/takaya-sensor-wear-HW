#ifndef UC20G_h
#define UC20G_h
#include <SoftwareSerial.h>

class UC20G
{
  public:
    UC20G(SoftwareSerial *ser);
    
    //初期化用の関数
    //AT+QICSGP=1,1,"SORACOM.IO","sora","sora",0
    //AT+QIDEACT=1
    bool init(String apn, String user, String pass);
    bool disconnect();
    bool disable();
    bool udpSend(String ipAdress, String port, const char* message);
    bool at();
  private:
    SoftwareSerial *uc20SwSerial;
    void clearSerialBuffer();
};

#endif
