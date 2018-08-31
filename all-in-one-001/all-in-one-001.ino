#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

float headingDegrees;
int count=0;
String str1, str2, str3, allStr;

void setupSerial(){
  Serial.begin(9600);
  Serial.write("ONE-TIME-ONLY\n");
  Serial1.begin(9600);
  Serial1.write("ONE-TIME-ONLY\n");
  Serial2.begin(9600);
  Serial2.write("ONE-TIME-ONLY\n");  
}

void setupHeading(){
  

  if(!mag.begin()){
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
}

void getHeading(){
  
  sensors_event_t event; 
  mag.getEvent(&event);
  float magx, magy;
  magx = event.magnetic.x;
  magy = event.magnetic.y;
  magx = magx + 2.3;
  magy = magy + 17;
  Serial.print(magx); Serial.print("\t");
  Serial.print(magy); Serial.print("\t");
  float heading = atan2(magy, magx);
  float declinationAngle = -10;
  if(heading < 0)
    //heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  headingDegrees = (heading * ( (180 + declinationAngle)/M_PI) ) + declinationAngle; 
  headingDegrees = headingDegrees * 1;
  Serial.println(headingDegrees);
  //for(;;){}  
  
}

void serialSend(){
  allStr = "";
   str1 = "EVERY 1 SECOND: " ;
   str2 = String(count);
   str3 =  "\n";
   allStr.concat(str1);
   allStr.concat(str2);
   allStr.concat(str3);
   int bytesSent = Serial.print( allStr );
   int bytesSent1 = Serial1.print( allStr );
   int bytesSent2 = Serial2.print( allStr );
   count++;
   Serial.flush();
   Serial1.flush();
   Serial2.flush();  

}

void setup() {
  setupSerial();
  setupHeading();

}
void loop() {
  // put your main code here, to run repeatedly:
  getHeading();
  serialSend();
  delay(500);
}
