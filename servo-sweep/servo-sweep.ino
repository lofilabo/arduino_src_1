 
#include <Servo.h> 
#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); 
int servoPin = 12;
 
Servo servo;  
 
int angle = 0;   // servo position in degrees 
 
void setup() 
{ 
  servo.attach(servoPin); 
  Serial.begin(115200);
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
  
} 
 
void getMeasurement(){
  
  
  VL53L0X_RangingMeasurementData_t measure;
    
  //Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    //Serial.print("Distance (mm): "); 
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
    }


void loop() 
{ 
  //servo.write(90);
  
  // scan from 0 to 180 degrees
  for(angle = 45; angle < 135; angle++)  
  {                                  
    servo.write(angle); 
    Serial.print(angle);
    Serial.print("\t");
    getMeasurement();              
    delay(20);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 135; angle > 45; angle--)    
  {                                
    servo.write(angle);      
    Serial.print(angle);
    Serial.print("\t");
    getMeasurement();        
    delay(20);       
  } 
  
} 
