
// CAN Receive Example
//POD - SLOW!!! VERSION
//
//      if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)
//      CAN_250KBPS for SLOW CAN BUS
//    
//      Wire.begin(0x7C I2C address #124/0x7C for SLOW CAN board
//      
//        MCP_8MHZ  <-- 8MHZ Crystal in little CAN boards and homemade CAN board
//        MCP_16MHZ <-- 16MHZ Crystal in PiCAN, Arduino CAN Shield
//
//        LOWER   7B
//        UPPER   91
//

#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

//byte steeringAngle[4] = {0};
//byte forwardSpeed[4] = {0};
int8_t txBuffer[2] = {0};
char allDataOut[10];
#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10


int convert_steering_angle_to_degrees_decimal( unsigned char upperByte , unsigned char lowerByte){

  signed short int result;
  result = (upperByte << 8) | lowerByte;
  result = result/1024.0;
  txBuffer[0] = (byte)(result);
  //itoa(result/1024.0, steeringAngle, 10);  
  return result/1024.0;   
   
}

int convert_speed_to_mps_decimal( unsigned char upperByte , unsigned char lowerByte){

  signed short int result;
  result = (upperByte << 8) | lowerByte;
  result = result/1024.0;
  txBuffer[1] = (byte)(result);
  //itoa(result/1024.0, forwardSpeed, 10);  
  return result/1024.0;    
  
}

void requestEvent_SendSteeringAngleAndSpeed() {
  //char* concatcharacter = "|";
  
  Wire.write( (byte*)txBuffer, 2 );
  //Wire.write(steeringAngle, 8);
  //Wire.write( "|" ); 
  //Wire.write( forwardSpeed );
  

  //Wire.write( allDataOut );
}

void setup()
{ 
  
  Serial.begin(9600);

  Wire.begin(0x7C);                // join i2c bus with address #124/0x7C
  Wire.onRequest(requestEvent_SendSteeringAngleAndSpeed); // register event
    
  // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                           // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");
}

void loop()
{

  signed short int resultSTEER;
  signed short int resultSPEED;
  unsigned char uByte_STEER = 0x00;
  unsigned char lByte_STEER = 0x00;
  unsigned char uByte_SPEED = 0x00;
  unsigned char lByte_SPEED = 0x00;

  
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)

    
/*    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
*/
    if ( rxId == 0x700 ){
      uByte_SPEED = rxBuf[0];
      lByte_SPEED = rxBuf[0];
      
      uByte_STEER = rxBuf[3];
      lByte_STEER = rxBuf[2];
      
      resultSTEER = convert_steering_angle_to_degrees_decimal( uByte_STEER, lByte_STEER );
      resultSPEED = convert_speed_to_mps_decimal( uByte_SPEED, lByte_SPEED );
        //Serial.print(msgString);
        //Serial.print( rxBuf[2] );
        //Serial.print ("\t");
        //Serial.print( rxBuf[3] );
        //Serial.print ("\t");
        Serial.print( "stA: " );
        Serial.print( txBuffer[0], DEC );
        Serial.print( "°" );
        Serial.print ("\t");

        Serial.print( "fSP: " );
        Serial.print( txBuffer[1], DEC );
        Serial.print( "mps" );
        Serial.print ("\t");


       Serial.println();
    }

  /*
        if ( rxId == 0x700 ){
            Serial.print(rxId);
            Serial.print("\t");
            
                if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
                  sprintf(msgString, " REMOTE REQUEST FRAME");
                  Serial.print(msgString);
                } else {
                  for(byte i = 0; i<len; i++){
                    sprintf(msgString, " 0x%.2X", rxBuf[i]);
                    Serial.print(msgString);
                  }
                }
            
                    Serial.println();
        }
*/
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
