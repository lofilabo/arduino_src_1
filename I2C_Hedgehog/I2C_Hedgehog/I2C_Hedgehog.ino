#include <Wire.h>
#include <Adafruit_MCP4725.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(500000);  // start serial for output
}

void loop() {
  Wire.requestFrom(0x6A , 64);    // request 6 bytes from slave device #0x6A 
  //Serial.println( Wire.available() ); 
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  Serial.println("");   
  delay(0);
  
 
}
