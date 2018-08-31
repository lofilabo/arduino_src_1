#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  //dac.begin(0x62);
  //Serial.println("SETUP"); 
}

void loop() {

  getSteeringAngle();

}

void getSteeringAngle(){
  
  
  int i=0;
  char buf[4];
  Wire.requestFrom(0x7C, 4);    // request 4 bytes from slave device #0x7C / #124
  //Serial.println( Wire.available() ); 
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    buf[i]=c;
    i++;
    Serial.print(c);         // print the character
   
  }
  Serial.print(" : ");
  Serial.println( atoi(buf) );   
}
