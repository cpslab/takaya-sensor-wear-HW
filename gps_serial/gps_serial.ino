//#include <SoftwareSerial.h>

//HardwareSerial mySerial(2);
//SoftwareSerial mySerial(2, 3);
HardwareSerial mySerial(2);

typedef struct {
  float La;
  String latitude;
  float Lo;
  String longitude;
} gpsdata;

String dataes[10];
uint8_t j = 0;
bool flag = false;

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
  uint8_t i;
  for (i = 0; i < 10; i++) {
    dataes[i] = "";
  }
  mySerial.print("$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n");
}

void loop() {
  // put your main code here, toweb run repeatedly:
  printGPS(getGPS());
  //    getGPS();
  //  if (mySerial.available()) {
  //    Serial.write(mySerial.read());
  //  }
}

gpsdata getGPS() {
  uint8_t i, n;
  gpsdata GPS_data;
  char origin[64];
  n = 0;
  while (mySerial.available()) {
    origin[n] = (char)mySerial.read();
    n++;
  }
  for (i = 0; i < n; i++) {
    if (j == 10) {
      for (i = 0; i < 10; i++) {
        dataes[i] = "";
      }
      j = 0;
    }
    if (origin[i] == '$') {
      dataes[j] += origin[i];
      flag = true;
      continue;
    }
    if (origin[i] == '\n' || origin[i] == '\r') {
      dataes[j] += '\n';
      j++;
      continue;
    }
    if (flag) {
      dataes[j] += origin[i];
    }
  }
  for (i = 0; i < j; i++) {
    if (dataes[i].startsWith("$GPGGA")) {
//              Serial.print(dataes[i]);
      String gpgga = dataes[i].substring(7);
      //                Serial.println(gpgga);
      String d[4] = {"", "", "", ""};
      d[0] = gpgga.substring(11, 20); //緯度
      d[1] = gpgga.substring(21, 22); //北緯、南緯
      d[2] = gpgga.substring(23, 33); //経度
      d[3] = gpgga.substring(34, 35); //東経、西経
      if (d[0].startsWith(",")) {
        Serial.println("false");
        GPS_data.La = 0.;
        GPS_data.latitude = "";
        GPS_data.Lo = 0.;
        GPS_data.longitude = "";
        return GPS_data;
      } else {
        if (d[1].equals("N") || d[1].equals("S")) {
          String la[2];
          String lo[2];
          la[0] = d[0].substring(0, 2);
          la[1] = d[0].substring(2);
          lo[0] = d[2].substring(0, 3);
          lo[1] = d[2].substring(3);

          if (d[1].equals("N")) {
            if (d[3].equals("E")) {
              GPS_data.La = la[0].toFloat() + la[1].toFloat() / 60.0;
              GPS_data.latitude = d[1];
              GPS_data.Lo = lo[0].toFloat() + lo[1].toFloat() / 60.0;
              GPS_data.longitude = d[3];
              return GPS_data;
            } else {
              GPS_data.La = la[0].toFloat() + la[1].toFloat() / 60.0;
              GPS_data.latitude = d[1];
              GPS_data.Lo = -(lo[0].toFloat() + lo[1].toFloat() / 60.0);
              GPS_data.longitude = d[3];
              return GPS_data;
            }
          } else {
            if (d[3].equals("E")) {
              GPS_data.La = -(la[0].toFloat() + la[1].toFloat() / 60.0);
              GPS_data.latitude = d[1];
              GPS_data.Lo = lo[0].toFloat() + lo[1].toFloat() / 60.0;
              GPS_data.longitude = d[3];
              return GPS_data;
            } else {
              GPS_data.La = -(la[0].toFloat() + la[1].toFloat() / 60.0);
              GPS_data.latitude = d[1];
              GPS_data.Lo = -(lo[0].toFloat() + lo[1].toFloat() / 60.0);
              GPS_data.longitude = d[3];
              return GPS_data;
            }
          }

        } else {
          Serial.println("false22");
          GPS_data.La = 0.;
          GPS_data.latitude = "";
          GPS_data.Lo = 0.;
          GPS_data.longitude = "";
          return GPS_data;
        }
        //          Serial.println(d[0]);
        //          Serial.println(d[1]);
        //          Serial.println(d[2]);
        //          Serial.println(d[3]);

      }
    }
  }
}

void printGPS(gpsdata g) {
  if (g.latitude != "") {
    Serial.println(g.La);
    Serial.println(g.latitude);
    Serial.println(g.Lo);
    Serial.println(g.longitude);
  }
}

