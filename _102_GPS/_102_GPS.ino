/*

 Designer Systems DS-GPAM Shield Arduino Demonstrator
 
 Requires Arduino UNO or MEGA or NANO boards
 
 DS-GPAM.S on I2C interface (ANALOG IN 4 [SDA] & 5 [SCL]) outputs data into Arduino
 serial terminal window at 9600 baud
 
 GPAMS_Application.pde  Date: 25/2/15 Version: 1.00
 
 */

#include <Wire.h>

#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10);     // Set CS to pin 10

#define GPAM 0x68                                 // GPM I2C Address

byte Address;                                         // Global variable
byte Data;                                            // Global variable

float data_degrees_lat = 0.00;
int data_degrees_lat_1 = 0;
int data_degrees_lat_2 = 0;

byte allData_mins_lat[8];
float data_mins_lat = 0.00;
int data_mins_lat_1 = 0;
int data_mins_lat_2 = 0;
int data_mins_lat_3 = 0;
int data_mins_lat_4 = 0;
int data_mins_lat_5 = 0;
int data_mins_lat_6 = 0;
int data_mins_lat_7 = 0;
int data_mins_lat_8 = 0;
float data_mins_lat_as_degrees = 0.00;
float data_all_lat_as_degrees = 0.00;


float data_degrees_long = 0.00; //why does this have a 0 and the lat group doesnt?  LONG has 3 degrees elements!
int data_degrees_long_0 = 0;
int data_degrees_long_1 = 0;
int data_degrees_long_2 = 0;

byte allData_mins_long[8];
float data_mins_long = 0.00;
int data_mins_long_0 = 0; 
int data_mins_long_1 = 0;
int data_mins_long_2 = 0;
int data_mins_long_3 = 0;
int data_mins_long_4 = 0;
int data_mins_long_5 = 0;
int data_mins_long_6 = 0;
int data_mins_long_7 = 0;
int data_mins_long_8 = 0;
float data_mins_long_as_degrees = 0.00;
float data_all_long_as_degrees = 0.00;
long data_mins_long_as_degrees_l = 0;
long data_all_long_as_degrees_l = 0;

int latDir=0;
int longDir=0;




void float2Bytes(float val,byte* bytes_array){
  // Create union of shared memory space
  union {
    float float_variable;
    byte temp_array[8];
  } u;
  // Overite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(bytes_array, u.temp_array, 4);
}




void setup()
{

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

  
  Wire.begin();                                       // Start I2C comms
  Serial.begin(9600);                                 // Start PC serial port @ 9600 baud
  delay(3000);                                        // Wait 3 seconds for DS-GPAM.S to initialise

}

void getHeading(){
  
  Serial.print("Heading: ");
  Address = 44;                                       // Point to Heading 1 register
  Data = GetDouble();                                 // Read registers from GPM
  Serial.print(Data, DEC);                            // and print to PC
  Address = 46;                                       // Point to Heading 2 register
  Data = GetSingle();                                 // Read registers from GPM
  Serial.print(Data, DEC);                            // and print to PC
  Serial.print(".");
  Address = 47;                                       // Point to Heading 3 register
  Data = GetSingle();                                 // Read registers from GPM
  Serial.println(Data, DEC);                          // and print to PC


}


void getSpeed(){

  Serial.print("Speed: ");
  Address = 52;                                       // Point to Speed 1 register
  Data = GetDouble();                                 // Read registers from GPM
  Serial.print(Data, DEC);                            // and print to PC
  Address = 54;                                       // Point to Speed 2 register
  Data = GetSingle();                                 // Read registers from GPM
  Serial.print(Data, DEC);                            // and print to PC
  Serial.print(".");
  Address = 55;                                       // Point to Speed 3 register
  Data = GetSingle();                                 // Read registers from GPM
  Serial.println(Data, DEC);                          // and print to PC

  delay(2000);                                        // Wait 2 seconds

  
}

void getTime(){


    for(int x=0; x <=2; x++){
      
        Serial.print("Time: ");
        Address = 0;                                        // Point to Hours register
        Data = GetDouble();                                 // Read registers from GPM
        Serial.print(Data, DEC);                            // and print to PC
        Serial.print(":");
        Address = 2;                                        // Point to Minutes register
        Data = GetDouble();                                 // Read registers from GPM
        Serial.print(Data, DEC);                            // and print to PC
        Serial.print(":");
        Address = 4;                                        // Point to Seconds register
        Data = GetDouble();                                 // Read registers from GPM
        Serial.println(Data, DEC);                          // and print to PC
      
        Serial.print("Date: ");
        Address = 6;                                        // Point to Day register
        Data = GetDouble();                                 // Read registers from GPM
        Serial.print(Data, DEC);                            // and print to PC
        Serial.print("/");
        Address = 8;                                        // Point to Month register
        Data = GetDouble();                                 // Read registers from GPM
        Serial.print(Data, DEC);                            // and print to PC
        Serial.print("/");
        Address = 10;                                       // Point to Year 1 register
        Data = GetDouble();                                 // Read registers from GPM
        Serial.print(Data, DEC);                            // and print to PC
        Address = 12;                                       // Point to Year 2 register
        Data = GetDouble();                                 // Read registers from GPM
        Serial.println(Data, DEC);                          // and print to PC
        delay(1000);                                        // Wait a second
        }
  
}


float getLatAsDegreesOnly(){

  
  data_degrees_lat = 0.00;
  data_degrees_lat_1 = 0;
  data_degrees_lat_2 = 0;

  data_mins_lat = 0.00;
  data_mins_lat_1 = 0;
  data_mins_lat_2 = 0;
  data_mins_lat_3 = 0;
  data_mins_lat_4 = 0;
  data_mins_lat_5 = 0;
  data_mins_lat_6 = 0;
  data_mins_lat_7 = 0;
  data_mins_lat_8 = 0;
  data_mins_lat_as_degrees = 0.00;
  data_all_lat_as_degrees = 0.00;

 
  //Degrees Section
  Address = 14;                                       // Point to Latitude 1 register
  Data = GetSingle();                                 // Read registers from GPM
  data_degrees_lat_1 = Data;
  Address = 15;                                       // Point to Latitude 2 register
  Data = GetSingle();                                 // Read registers from GPM
  data_degrees_lat_2 = Data;
  //Serial.println( Data );
  //Minutes section
  Address = 16;                                       // Point to Latitude 3 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_lat_1 = Data;
  Address = 17;                                       // Point to Latitude 4 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_lat_2 = Data;
  
  //decimal point happens here!
  
  Address = 18;                                       // Point to Latitude 5 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_lat_3 = Data;
  Address = 19;                                       // Point to Latitude 6 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_lat_4 = Data;
  Address = 20;                                       // Point to Latitude 7 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_lat_5 = Data;
  Address = 21;                                       // Point to Latitude 8 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_lat_6 = Data;
  Address = 22;                                       // Point to Latitude character register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_lat_7 = Data;


  data_degrees_lat = data_degrees_lat + ( data_degrees_lat_1 * 10 );
  data_degrees_lat = data_degrees_lat + ( data_degrees_lat_2 * 1 );

  data_mins_lat = data_mins_lat + ( data_mins_lat_1 * 10 );
  data_mins_lat = data_mins_lat + ( data_mins_lat_2 * 1 );
  data_mins_lat = data_mins_lat + ( data_mins_lat_3 * 0.1 );
  data_mins_lat = data_mins_lat + ( data_mins_lat_4 * 0.01 );
  data_mins_lat = data_mins_lat + ( data_mins_lat_5 * 0.001 );
  data_mins_lat = data_mins_lat + ( data_mins_lat_6 * 0.0001 );
  data_mins_lat_as_degrees =  data_mins_lat / ( 60 );
  data_all_lat_as_degrees = data_degrees_lat + data_mins_lat_as_degrees;

  if( data_mins_lat_7 != 78 ){
    //78 = ASCII N.
    //if this register is not 78, it is SOUTH, and the whole value should be negetivised.
    data_all_lat_as_degrees = data_all_lat_as_degrees * -1;
    latDir=1;
  }else{
    latDir=0;
  }
  byte data1[8] = {latDir};
  byte sndStat1 = CAN0.sendMsgBuf(0x103, 0, 8, data1);
  if(sndStat1 == CAN_OK){
    Serial.print("CAN OK\t");
  } else {
    Serial.print("BAD CAN\t");
  }  
 // byteArray[0] = (int)((longInt & 0xFF000000) >> 24 );
 /*
 //THIS SEGEMENT BELOW HERE....
 byte data2[8];
 float2Bytes(data_all_lat_as_degrees,&data2[0]);
 //DOES EXACTLY THE SAME AS THE LAST SIX BYTES OF THIS PART BELOW.
 */
 /*
 Serial.println();
 Serial.println(data_mins_lat_1);
 Serial.println(data_mins_lat_2);
 Serial.println(data_mins_lat_3);
 Serial.println(data_mins_lat_4);
 Serial.println(data_mins_lat_5);
 Serial.println(data_mins_lat_6);
 Serial.println(data_mins_lat, DEC);
 */
  byte data2[8] = {
    data_degrees_lat_1, 
    data_degrees_lat_2, 
    data_mins_lat_1, 
    data_mins_lat_2, 
    data_mins_lat_3, 
    data_mins_lat_4, 
    data_mins_lat_5, 
    data_mins_lat_6
  };
 
  byte sndStat2 = CAN0.sendMsgBuf(0x104, 0, 8, data2);
  if(sndStat2 == CAN_OK){
    Serial.print("CAN DATA1 OK\t");
  } else {
    Serial.print("BAD CAN DATA1\t");
  }  
  return data_all_lat_as_degrees;
  
}




float getLongAsDegreesOnly(){

  
  data_degrees_long = 0.00;
  data_degrees_long_1 = 0;
  data_degrees_long_2 = 0;

  data_mins_long = 0.00;
  data_mins_long_1 = 0;
  data_mins_long_2 = 0;
  data_mins_long_3 = 0;
  data_mins_long_4 = 0;
  data_mins_long_5 = 0;
  data_mins_long_6 = 0;
  data_mins_long_7 = 0;
  data_mins_long_8 = 0;
  data_mins_long_as_degrees = 0.00;
  data_all_long_as_degrees = 0.00;

 
  //Degrees Section
  Address = 23;                                       // Point to Longitude 1 register
  Data = GetSingle();                                 // Read registers from GPM
  data_degrees_long_0 = Data;
  Address = 24;                                       // Point to Longitude 2 register
  Data = GetSingle();                                 // Read registers from GPM
  data_degrees_long_1 = Data;
  Address = 25;                                       // Point to Longitude 3 register
  Data = GetSingle();                                 // Read registers from GPM
  data_degrees_long_2 = Data;

  //Minutes section
  Address = 26;                                       // Point to Longitude 4 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_long_1 = Data;
  Address = 27;                                       // Point to Longitude 5 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_long_2 = Data;
  
  //decimal point happens here!
  
  Address = 28;                                       // Point to Longitude 6 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_long_3 = Data;
  Address = 29;                                       // Point to Longitude 7 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_long_4 = Data;
  Address = 30;                                       // Point to Longitude 8 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_long_5 = Data;
  Address = 31;                                       // Point to Longitude 9 register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_long_6 = Data;
  Address = 32;                                       // Point to Longitude character register
  Data = GetSingle();                                 // Read registers from GPM
  data_mins_long_7 = Data;


  data_degrees_long = data_degrees_long + ( data_degrees_long_0 * 100 );
  data_degrees_long = data_degrees_long + ( data_degrees_long_1 * 10 );
  data_degrees_long = data_degrees_long + ( data_degrees_long_2 * 1 );

  data_mins_long = data_mins_long + ( data_mins_long_1 * 10 );
  data_mins_long = data_mins_long + ( data_mins_long_2 * 1 );
  data_mins_long = data_mins_long + ( data_mins_long_3 * 0.1 );
  data_mins_long = data_mins_long + ( data_mins_long_4 * 0.01 );
  data_mins_long = data_mins_long + ( data_mins_long_5 * 0.001 );
  data_mins_long = data_mins_long + ( data_mins_long_6 * 0.0001 );
  //data_mins_long = data_mins_long + ( data_mins_long_7 * 0.00001 );
  data_mins_long_as_degrees =  data_mins_long / ( 60 );
  data_all_long_as_degrees = data_degrees_long + data_mins_long_as_degrees;

  if( data_mins_long_7 != 45 ){
    //45 = ASCII E.
    //if this register is not 78, it is WEST, and the whole value should be negetivised.    
    data_all_long_as_degrees = data_all_long_as_degrees * -1;
    longDir = 1;
  }else{
    longDir = 0;  
  }

  byte data1[8] = {longDir};
  byte sndStat1 = CAN0.sendMsgBuf(0x105, 0, 8, data1);
  if(sndStat1 == CAN_OK){
    Serial.print("CAN OK\t");
  } else {
    Serial.print("BAD CAN\t");
  }  
  byte data2[8] = {
    data_degrees_long_0, 
    data_degrees_long_1, 
    data_degrees_long_2, 
    data_mins_long_1, 
    data_mins_long_2, 
    data_mins_long_3, 
    data_mins_long_4, 
    data_mins_long_5
  };
  byte sndStat2 = CAN0.sendMsgBuf(0x106, 0, 8, data2);
  if(sndStat2 == CAN_OK){
    Serial.print("CAN DATA1 OK\t");
  } else {
    Serial.print("BAD CAN DATA1\t");
  }  

  return data_all_long_as_degrees;
  
}

void  getReg_14_15(){
  Address = 14;                                       // Point to Latitude 1 register
  Data = GetSingle();                                 // Read registers from GPM
  //Serial.print(Data, DEC);                            // and print to PC
  Address = 15;                                       // Point to Latitude 2 register
  Data = GetSingle();                                 // Read registers from GPM
  //Serial.print(Data, DEC);                            // and print to PC
  //Serial.print("\n");
}

void loop(){
  getReg_14_15();
  //getSpeed();
  //getHeading();
  //getTime();
  Serial.print(  getLatAsDegreesOnly() , DEC );
  Serial.print (  "\t" );
  Serial.print(  getLongAsDegreesOnly() , DEC );
  Serial.print("\n");
  
  delay(10000);

}

int GetDouble(){              // Get double register value from GPAM

  int Value = 0; 
  byte H_Byte = 0;
  byte L_Byte = 0;

  Wire.beginTransmission(GPAM);
  Wire.write(Address);              // Send register start address
  Wire.endTransmission();

  Wire.requestFrom(GPAM, 2);            // Request double register
  while(Wire.available() < 2);            // Wait for double byte
  H_Byte = Wire.read();                               // Store one
  L_Byte = Wire.read();                               // Store two

  Value = (H_Byte * 10) + L_Byte;                     // Adjust for one byte

  return(Value);                            
}


int GetSingle(){              // Get single register value from GPAM

  int Value = 0; 

  Wire.beginTransmission(GPAM);
  Wire.write(Address);              // Send register start address
  Wire.endTransmission();

  Wire.requestFrom(GPAM, 1);            // Request register
  while(Wire.available() < 1);            // Wait for single byte
  Value = Wire.read();                                // and store

  return(Value);                            
}

