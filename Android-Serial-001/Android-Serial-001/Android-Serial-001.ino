#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(10, 11); // RX, TX (TX not used)


void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop()
{
  static int i = 0;
  if (gpsSerial.available()) {
    char ch = gpsSerial.read();
    Serial.println(ch);
  }
}
