#include "Adafruit_VL53L0X.h"
#include <Servo.h>
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int disregardSample = 0;
long totalDistance=0;
int j=0;
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int servoVal=0;
int servoDir=0;
int servoIncrement=2;

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(115200);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {

  if(servoVal > 180){
    servoDir = 1;
    servoVal = 180;
  }else if(servoVal < 1){
    servoDir = 0;
    servoVal = 1;
  }

  if(servoDir==1){
    servoVal = servoVal - servoIncrement;
  }else{
    servoVal = servoVal + servoIncrement;
  }

  VL53L0X_RangingMeasurementData_t measure;
  long distance;
  //Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
      if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        //Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
        distance = (int)measure.RangeMilliMeter;
        
      }else{
        distance=10000000;
      }
      totalDistance = totalDistance + distance;
      j++;
      if(j>9){
        myservo.write(servoVal); 
        Serial.print(servoVal);
        Serial.print(",");
        Serial.println( (totalDistance/10)  );//*0.128 );
        totalDistance = 0;
        j=0;
      }
    /*
    disregardSample = 0;
    totalDistance=0;
    int i;
    for(i=0; i<100; i++){
      if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        //Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
        //Serial.println(measure.RangeMilliMeter);
        
        totalDistance = totalDistance + measure.RangeMilliMeter;
      } else {
        //Serial.println(" out of range ");
        disregardSample = 1;
        
      }
      delay(0);
    }   

    if(disregardSample != 1){
       Serial.println(totalDistance/100);
    }else{
      //Serial.println("DISREGARDED");  
    }
    */
  
}
