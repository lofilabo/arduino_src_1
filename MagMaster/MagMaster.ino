#include "Wire.h"
//#include "HMC5883L.h"
#include <Adafruit_HMC5883_U.h>
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
//HMC5883L compass; //Copy the folder "HMC5883L" in the folder "C:\Program Files\Arduino\libraries" and restart the arduino IDE.

float xv, yv, zv;

void setup()
{   
  Serial.begin(9600);
  Wire.begin();  
  //compass = HMC5883L();  
  //setupHMC5883L(); 
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }      
}

void loop()
{
  getHeading();

  Serial.flush(); 
  Serial.print(xv); 
  Serial.print(",");
  Serial.print(yv);
  Serial.print(",");
  Serial.print(zv);
  Serial.println();

  delay(100); 
} 

void setupHMC5883L()
{  
  //compass.SetScale(0.88);
  //compass.SetMeasurementMode(Measurement_Continuous);
}
 
void getHeading()
{ 

  sensors_event_t event; 
  mag.getEvent(&event);
  
  float magx, magy;
  
  xv = event.magnetic.x*0.8;
  yv = event.magnetic.y*0.8;
  zv = event.magnetic.z*0.8;
  /*
  MagnetometerRaw raw = compass.ReadRawAxis();
  
  xv = (float)raw.XAxis;
  yv = (float)raw.YAxis;
  zv = (float)raw.ZAxis;
  */
}
