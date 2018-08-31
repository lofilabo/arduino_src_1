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


#define servoPin 8
#define motorPin 9


Servo Servo1;
Servo Servo2;
Servo Servo3;

float motorSpeed;
float servoAngle;

int sensorPin = A7;    // select the input pin for the potentiometer
float sensorValue = 0;  // variable to store the value coming from the sensor



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
  Serial2.begin(9600);
  //Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
    Servo1.attach(motorPin);
    Servo2.attach(servoPin);
    //Servo1.attach(motorPin);
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
  displaySensorDetails();
}

void loop(void) 
{

  sensorValue = analogRead(sensorPin);
  sensorValue = sensorValue * 2.1;
  Serial.println( sensorValue );
   if(sensorValue > 50){
      Servo1.write(70);
   }else{
      Servo1.write(92);
   }
  
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
  
  float magx, magy;
  
  magx = event.magnetic.x;
  magy = event.magnetic.y;

  //magx = magx - 2;
  //magy = magy + 17.5;
  
  magx = magx + 2.3;
  magy = magy + 17;

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print(magx); Serial.print("\t");
  Serial.print(magy); Serial.print("\t");
  //Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  //Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  //Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");
  //Serial.println("uT");
  
  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(magy, magx);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
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
  Serial2.println(headingDegrees);
  Serial2.flush();
  //Serial.print("Heading (radians): "); Serial.println(heading);
  //Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
  
  //for(;;){}
    
  processHeadingByTarget( headingDegrees );
    
/*
    if( headingDegrees < -2 ){
        servoAngle = 40;
        Serial.println("L");
    }else if( headingDegrees > 2 ){
        servoAngle = 140;
        Serial.println("R");
    }else{
        servoAngle = 97;
        Serial.println("C");
    }
    Servo2.write(servoAngle);
*/


  delay(100);
}


void processHeadingByTarget( float headingDegrees ){

    int targetHeading = -35;



    if(  headingDegrees < targetHeading-5  ){

      if(  headingDegrees < targetHeading-10  ){
        servoAngle = 57;
        Serial.println("LL");    
      }else{
        servoAngle = 94;
        Serial.println("L");    
      }
      
    }else if( headingDegrees > targetHeading+5 ){

      if( headingDegrees > targetHeading+10 ){
        servoAngle = 137; 
        Serial.println("RR");           
      }else{
        servoAngle = 100; 
        Serial.println("R");        
      }
    }else{
        servoAngle = 97;
        Serial.println("C");    
    }

/*  
    if( headingDegrees < -2 ){
        servoAngle = 40;
        Serial.println("L");
    }else if( headingDegrees > 2 ){
        servoAngle = 140;
        Serial.println("R");
    }else{
        servoAngle = 97;
        Serial.println("C");
    }
*/
    Servo2.write(servoAngle);  
  
}

