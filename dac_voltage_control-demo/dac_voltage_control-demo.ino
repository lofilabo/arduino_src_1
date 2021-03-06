#include <Wire.h>
#include <Adafruit_MCP4725.h>
#define voltsIn A0

Adafruit_MCP4725 dac; // constructor

void setup(void) {
  Serial.begin(9600);
  dac.begin(0x62); // The I2C Address: Run the I2C Scanner if you're not sure  
  
}

void loop(void) {
  
    uint32_t dac_value;
    int adcValueRead = 0;
    float voltageRead = 0;
    
    float dac_expected_output;
    
   
    for (dac_value = 0; dac_value < 4096; dac_value = dac_value + 15)
    {
      dac_expected_output = (5.0/4096.0) * dac_value;
      dac.setVoltage(dac_value, false);
      delay(250);
      adcValueRead = analogRead(voltsIn);
      voltageRead = (adcValueRead * 5.0 )/ 1024.0;
      
      Serial.print("DAC Value: ");
      Serial.print(dac_value);
      
      Serial.print("\tExpected Voltage: ");
      Serial.print(dac_expected_output,3);
      
      Serial.print("\tArduino ADC Value: ");
      Serial.print(adcValueRead);
      
      Serial.print("\tArduino Voltage: ");      
      Serial.println(voltageRead,3);      
    }    
}
