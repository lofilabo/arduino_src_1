/**************************************************************************/
/*! 
    @file     trianglewave.pde
    @author   Adafruit Industries
    @license  BSD (see license.txt)

    This example will generate a triangle wave with the MCP4725 DAC.   

    This is an example sketch for the Adafruit MCP4725 breakout board
    ----> http://www.adafruit.com/products/935
 
    Adafruit invests time and resources providing this open source code, 
    please support Adafruit and open-source hardware by purchasing 
    products from Adafruit!
*/
/**************************************************************************/



/*
X = CHANNEL A = THROTTLE - UN-USED.

Y = CHANNEL B = STEERING

*/

#include <Wire.h>
#include <Adafruit_MCP4725.h>


int sensorPinX = A8;   
float sensorValueX = 0; 
float newsensorValueX = 0;
int vOut;
int sensorPinS = 0;   
int sensorValueS = 0; 
Adafruit_MCP4725 dac;

void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello!");

  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  dac.begin(0x61);
    
  //Serial.println("Generating a triangle wave");
  Serial.println("Generating a DC Voltage");

     pinMode(22, OUTPUT);  //Initiates Relay pin
}

void loop(void) {

  sensorValueS = digitalRead(23);
 
    if(sensorValueS==1){
      sensorValueS = 1;
      digitalWrite(22, HIGH);
    }else{
      sensorValueS = 0;
      digitalWrite(22, LOW);
    }
    uint32_t counter;
    /*
    // Run through the full 12-bit scale for a triangle wave
    for (counter = 0; counter < 4095; counter++){
      dac.setVoltage(counter, false);
    }
    for (counter = 4095; counter > 0; counter--){
      dac.setVoltage(counter, false);
    }
    */

    /* Set a certain Voltage */
    newsensorValueX = analogRead(sensorPinX);
    Serial.print(newsensorValueX);
    Serial.print("\t");
    vOut = newsensorValueX * 4;

    if (vOut > 4094){
      vOut = 4094;
    }
    
   
    Serial.println(0);
    
    dac.setVoltage(vOut, false);
    delay(100);
}
