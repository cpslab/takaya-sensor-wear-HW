#ifndef GPS_TO_I2C_h
#define GPS_TO_I2C_h
#include <SoftwareSerial.h>

typedef struct{
    String La;
    float latitude;
    String Lo;
    float longitude;
} gpsdata;


class GPS_TO_I2C
{
public:
    GPS_TO_I2C(SoftwareSerial *ser);
    
    bool init();
    bool get_gps(gpsdata *gps);
private:
    SoftwareSerial *gpsSwSerial;
    gpsdata *gpsData;
}

#include <stdio.h>

#endif /* GPS_TO_I2C_h */
