/***************************************************************************
  This is a library example for the HMC5883 magnentometer/compass

  Designed specifically to work with the Adafruit HMC5883 Breakout
  http://www.adafruit.com/products/1746
 
  *** You will also need to install the Adafruit_Sensor library! ***

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries with some heading example from
  Love Electronics (loveelectronics.co.uk)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the version 3 GNU General Public License as
 published by the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ***************************************************************************/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Servo.h>


#define servoPin 22
#define motorPin 23


Servo Servo1;
Servo Servo2;
Servo Servo3;

float motorSpeed;
float servoAngle;



/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);

  //Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");
  //Serial.println("uT");  
  
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  
  delay(500);
}



void setup(void) 
{
  Serial.begin(9600);
 Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
    Servo1.attach(motorPin);
    Servo2.attach(servoPin);
    Servo1.attach(motorPin);
        Servo1.write(88.5);
            delay(3000);                    // Allow the ECU to initialize    
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  //displaySensorDetails();
}

void loop(void) 
{

  sensors_event_t event; 
  mag.getEvent(&event);
  
  float magx, magy;
  
  magx = event.magnetic.x;
  magy = event.magnetic.y;

  magx = magx + 0;    //    <------------------ x correction scalar
  magy = magy + 0;    //    <------------------ y correction scalar

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print(magx); Serial.print("\t");
  Serial.print(magy); Serial.print("\t");
  //Serial.println("");
  //Serial.print("\t");

  float heading = atan2(magy, magx);
  float declinationAngle = -1.74;
  //heading += declinationAngle;
  //Serial.println(heading);
  
  // Correct for when signs are reversed.
  if(heading < 0)
    //heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
    
  // Convert radians to degrees for readability.
  float headingDegrees = (heading * ( (180 + declinationAngle)/M_PI) ) + declinationAngle; 
  
  Serial.println(headingDegrees);
  
  //Serial.print("Heading (radians): "); Serial.println(heading);
  //Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
  
  for(;;){}
    

  delay(100);
}



