// CAN Receive Example
//

#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

/*
 * MEGA = Pin 49 == Physical 35 (by standard diagram)
 * UNO / NANO = Physical 32 == D2 (by standard diagram)
 * 
 * We could connect to P32/D2 on the Mega, but the standard diagram
 * wants up to bunch all the SPI things nicely together, so let's keep 
 * the interrupt close to MISO/MOSI/SS/SCK.
 */
//#define CAN0_INT 2                              // Uno/Nano
#define CAN0_INT 49                              // Mega

/*
 * MEGA = Pin 53 == Physical 19 (by standard diagram)
 * UNO / NANO = Physical 10 == D14 (by standard diagram)
 * 
 * We could connect to P10/D19 on the Mega, but the standard diagram
 * wants up to bunch all the SPI things nicely together,
 */
//MCP_CAN CAN0(10);     // Set CS to pin 10
MCP_CAN CAN0(53);     // Set CS to pin 10



void setup()
{
  Serial.begin(9600);
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");
}

byte data[8] = {0x99};
int i=0;

void loop()
{
  if(i>10){
    // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
    byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
    if(sndStat == CAN_OK){
      Serial.println("Message Sent Successfully!");
    } else {
      Serial.println("Error Sending Message...");
    }
    i=0;
  }
  delay(100);
  i++;
  
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
    Serial.print(msgString);
  
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
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

