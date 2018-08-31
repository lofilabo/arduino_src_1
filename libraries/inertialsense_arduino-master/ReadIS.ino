/*
  Read InertialSense

  This is a basic example of how to read data from the uINS using the InertialSense serial protocol

  The circuit:
  - inertialSense port H4 attached to Serial port 1
  - Serial Monitor open on Serial port 0

  created 26 Jan 2018
  by James Jackson

  This example code is in the public domain.
*/

#include "ISComm.h"
#include <stddef.h>

// you can make this 512 size if memory is really tight
static uint8_t s_buffer[1024];
static is_comm_instance_t s_comm;

static void handleGpsMessage(gps_nav_t* gps)
{
  // We only requested the status message.
  // The first 8 bits are the number of satellites,
  // the second 8 bits are the status flags
  // the last 16 bits are the fix type
  uint8_t num_sattelites = gps->status & GPS_STATUS_NUM_SATS_USED_MASK;
  Serial.print ("num_sats: ");
  Serial.print (num_sattelites, DEC);

  uint16_t fix_type = gps->status & GPS_STATUS_FIX_TYPE_MASK;
  switch (fix_type)
  {
  case GPS_STATUS_FIX_TYPE_NO_FIX:
    Serial.println(",\tNo Fix!");
    break;
  case GPS_STATUS_FIX_TYPE_2D_FIX:
    Serial.println(",\t2D Fix!");
    break;
  case GPS_STATUS_FIX_TYPE_3D_FIX:
    Serial.println(",\t3D Fix!");
    break;
  case GPS_STATUS_FIX_TYPE_GPS_PLUS_DEAD_RECK:
    Serial.println(",\tDead Reckoning");
    break;
  case GPS_STATUS_FIX_TYPE_RTK_FLOAT:
    Serial.println(",\tRTK Float!");
    break;
  case GPS_STATUS_FIX_TYPE_RTK_FIX:
    Serial.println(",\tRTK Fix!");
    break;
  default:
    Serial.print(",\tUnknown Fix Type: ");
    Serial.println(fix_type, HEX);
  }
}

static void handleINSMessage(ins_1_t* ins)
{
  Serial.print("Lat: ");
  Serial.print(ins->lla[0]);
  Serial.print(", Lon: ");
  Serial.print(ins->lla[1]);
  Serial.print(", Alt: ");
  Serial.print(ins->lla[2]);
  Serial.print(", roll: ");
  Serial.print(ins->theta[0]);
  Serial.print(", pitch: ");
  Serial.print(ins->theta[1]);
  Serial.print(", yaw: ");
  Serial.println(ins->theta[2]);
}


void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println("initializing");

  // This buffer is going to be used to hold messages
  // as they come in
  s_comm.buffer = s_buffer;
  s_comm.bufferSize = sizeof(s_buffer);
  is_comm_init(&s_comm);

  // Stop all the broadcasts on the device
  int messageSize = is_comm_stop_broadcasts(&s_comm);
  Serial1.write(s_buffer, messageSize); // Transmit the message to the inertialsense

  // Ask for ins_1 message 10 times per second.  Ask for the whole thing, so
  // set 0's for the offset and size
  messageSize = is_comm_get_data(&s_comm, DID_INS_1, 0, 0, 100);
  Serial1.write(s_buffer, messageSize); // Transmit the message to the inertialsense

  // Get only the status field out of the GPS_NAV messages (number of satellites and fix type) once per second
  messageSize = is_comm_get_data(&s_comm, DID_GPS1_NAV, sizeof(uint32_t), offsetof(gps_nav_t, status), 1000);
  Serial1.write(s_buffer, messageSize); // Transmit the message to the inertialsense
}

void loop() {
  // read from port 1, and see if we have a complete inertialsense packet
  if (Serial1.available()) {
    uint8_t inByte = Serial1.read();

    // This function returns the DID of the message that was just parsed, we can then point the buffer to
    // The right function to handle the message.  We can use a cast to interpret the s_buffer as the
    // kind of message that we received
    uint32_t message_type = is_comm_parse(&s_comm, inByte);
    switch (message_type)
    {
      case DID_NULL:
        break;
      case DID_GPS1_NAV:
        handleGpsMessage((gps_nav_t*)s_buffer);
        break;
      case DID_INS_1:
        handleINSMessage((ins_1_t*)s_buffer);
        break;
      default:
        Serial.print("Got an unexpected message DID: ");
        Serial.println(message_type, DEC);
    }
  }
}
