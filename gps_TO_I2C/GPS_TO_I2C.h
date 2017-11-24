#ifndef GPS_TO_I2C_h
#define GPS_TO_I2C_h
#include <HardwareSerial.h>
#include <stdio.h>

typedef struct{
    String La;
    float latitude;
    String Lo;
    float longitude;
} gpsdata;


class GPS_TO_I2C
{
public:
    GPS_TO_I2C(HardwareSerial *ser);
    
    bool init();
    bool get_gps(gpsdata *gps);
private:
    HardwareSerial *gpsSwSerial;
    gpsdata *gpsData;
}

#endif /* GPS_TO_I2C_h */
