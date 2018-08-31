// CAN Send Example
//

#include <mcp_can.h>
#include <SPI.h>


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
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
}

byte data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

void loop()
{
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(100);   // send data per 100ms
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
