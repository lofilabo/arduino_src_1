#include <Adafruit_HMC5883_U.h>

#include "Wire.h"
#include <Adafruit_HMC5883_U.h>
#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10);     // Set CS to pin 10
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

//HMC5883L compass; //Copy the folder "HMC5883L" in the folder "C:\Program Files\Arduino\libraries" and restart the arduino IDE.

float xv, yv, zv;
float pi = 3.14 ;
float headingAngleRadians;
int headingAngleDegrees;
float headingAngleRadiansWithDir;
int headingAngleDegreesWithDir;
int headingAngleDegreesWithoutDir;

int NorS;//N=0, S=1;
int EorW;//E=0, W=1;

//calibrated_values[3] is the global array where the calibrated data will be placed
//calibrated_values[3]: [0]=Xc, [1]=Yc, [2]=Zc
float calibrated_values[3];   



void loop(){

  getAndShowHeadings();
  
} 

//transformation(float uncalibrated_values[3]) is the function of the magnetometer data correction 
//uncalibrated_values[3] is the array of the non calibrated magnetometer data
//uncalibrated_values[3]: [0]=Xnc, [1]=Ync, [2]=Znc
void transformation(float uncalibrated_values[3])    
{
  //calibration_matrix[3][3] is the transformation matrix
  //replace M11, M12,..,M33 with your transformation matrix data
  double calibration_matrix[3][3] = 
  {
      {1.141, 0.043, -0.024},
      {0.015, 1.13, -0.058},
      {0.083, 0.017, 1.095}  
  };
  //bias[3] is the bias
  //replace Bx, By, Bz with your bias data
  double bias[3] = 
  {
      2.721,
      -7.952,
      -9.332
  };  
  //calculation
  for (int i=0; i<3; ++i) uncalibrated_values[i] = uncalibrated_values[i] - bias[i];
  float result[3] = {0, 0, 0};
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      result[i] += calibration_matrix[i][j] * uncalibrated_values[j];
  for (int i=0; i<3; ++i) calibrated_values[i] = result[i];
}

void setup(){
     
    Serial.begin(9600);
    Wire.begin();  
    
    if(!mag.begin()){
      Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
      while(1);
    } 
    
    // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
    if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
    else Serial.println("Error Initializing MCP2515...");
    
    CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be 
       
}


void getAndShowHeadings(){
  
  float values_from_magnetometer[3];

  String dir = "";

  getHeading();
  values_from_magnetometer[0] = xv;
  values_from_magnetometer[1] = yv;
  values_from_magnetometer[2] = zv;
  transformation(values_from_magnetometer);

  /*
  CALIBRATED VALUES [0] and [1]
  
  HEADING ANGLE = arctan ( [0]/[1] )
  
  How [0] and [1] change by quadrant:
          -,+           +, +
                  N
            NE    |   NW
                  |
          E --------------W
                  |
                  |
            SE    |   SW
                  S     
          -,+            -,-
  
  HOW WE CALCULATE FINAL HEADING:
  NW & NE are always 0 -> pi/2
  SW and SE are always pi/2 -> pi
  
  East Side is +ve
  West Side is -ve.
  */

  if(       calibrated_values[0] >= 0 && calibrated_values[1] < 0  ){
    dir = "SE";
    NorS = 1;
    EorW = 0;
  }else if( calibrated_values[0] < 0  && calibrated_values[1] < 0  ){
    dir = "SW";
    NorS = 1;
    EorW = 1;
  }else if( calibrated_values[0] < 0  && calibrated_values[1] >= 0  ){
    dir = "NW";
    NorS = 0;
    EorW = 1;
  }else if( calibrated_values[0] >= 0 && calibrated_values[1] >= 0 ){
    dir = "NE";
    NorS = 0;
    EorW = 0;
  }

  headingAngleRadians = atan(calibrated_values[0]/calibrated_values[1] );

  if ( headingAngleRadians < 0 ){
    headingAngleRadians = headingAngleRadians * -1;
  }
  
  if( NorS == 0){
    //take no action
  }else{
    headingAngleRadians = pi - headingAngleRadians; 
  }
  
  if( EorW == 0){
    //take no action
    headingAngleRadiansWithDir = headingAngleRadians;
  }else{
      headingAngleRadiansWithDir = headingAngleRadians * -1;
  }

  headingAngleDegreesWithDir = headingAngleRadiansWithDir * (180/pi);

  if( headingAngleDegreesWithDir < 0 ){
    headingAngleDegreesWithoutDir = headingAngleDegreesWithDir * -1;
  }else{
     headingAngleDegreesWithoutDir = headingAngleDegreesWithDir;
  }
  Serial.flush(); 
  Serial.print(calibrated_values[0]); 
  Serial.print("\t");
  Serial.print(calibrated_values[1]);
  Serial.print("\t");
  Serial.print(calibrated_values[2]);
  Serial.print( "\t---->\t" );
  Serial.print(  headingAngleRadiansWithDir );
  Serial.print( "\t---->\t" );
  Serial.print(  headingAngleDegreesWithDir );
  Serial.print("\t");
  Serial.print( dir );
  Serial.print("\t");
  Serial.print(  headingAngleDegreesWithoutDir );


  byte data[8] = {EorW, headingAngleDegreesWithoutDir};
  //byte data[8] = {1, 2};
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(0x101, 0, 8, data);
  if(sndStat == CAN_OK){
    Serial.print("\t CAN OK");
  } else {
    Serial.print("\t CAN BAD");
  }
    Serial.println();
  delay(250); 
  
  
}




 
void getHeading()
{ 
  sensors_event_t event; 

    mag.getEvent(&event);

  
  xv = event.magnetic.x;
  yv = event.magnetic.y;
  zv = event.magnetic.z;
}
