#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  dac.begin(0x62);
  //Serial.println("SETUP"); 
}

void loop() {
  Wire.requestFrom(0x6C, 32);    // request 6 bytes from slave device #0x6C / #108
  //Serial.println( Wire.available() ); 
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  Serial.println("");   
  delay(0);
  
  Wire.requestFrom(0x6D, 6);    // request 6 bytes from slave device #0x6D / 109
  //Serial.println( Wire.available() ); 
  while (Wire.available()) { // slave may send less than requested
    char d = Wire.read(); // receive a byte as character
    Serial.print(d);         // print the character
  }
  Serial.println("");   
  delay(0);
}
