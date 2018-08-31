#include <data_sets.h>
#include <ISComm.h>
#include <ISConstants.h>

#include "ISComm.h"
#include <stddef.h>

// static buffer for comm read / write
static uint8_t s_buffer[1024];

// create an instance to hold communications state
static is_comm_instance_t s_comm;

void setup() {

  // initialize both serial ports:
  Serial.begin(115200);
  Serial1.begin(115200);

  // assign buffer to comm instance - do this right before you call is_comm_init
  comm.buffer = s_buffer;
  comm.bufferSize = sizeof(s_buffer);

  // initialize comm interface - call this before doing any comm functions
  is_comm_init(&comm);

  // Configure the device as an RTK rover
  uint32_t sys_config = SYS_CFG_BITS_RTK_ROVER;
  messageSize = is_comm_set_data(&comm, DID_FLASH_CONFIG, sizeof(uint32_t), offsetof(nvm_flash_cfg_t, sysCfgBits), &sys_config);
  Serial1.write(comm.buffer, messageSize); // Transmit the message to the inertialsense device

  // Stop all the broadcasts on the device
  int messageSize = is_comm_stop_broadcasts(&comm);
  Serial1.write(comm.buffer, messageSize); // Transmit the message to the inertialsense device

  // Ask for ins_1 message 5 times per second.  Ask for the whole thing, so
  // set 0's for the offset and size
  messageSize = is_comm_get_data(&comm, DID_INS_1, 0, 0, 200);
  Serial1.write(comm.buffer, messageSize); // Transmit the message to the inertialsense device

  // Get only the status field out of the GPS_NAV messages (number of satellites and fix type) once per second
  messageSize = is_comm_get_data(&comm, DID_GPS1_NAV, sizeof(uint32_t), offsetof(gps_nav_t, status), 1000);
  Serial1.write(comm.buffer, messageSize); // Transmit the message to the inertialsense device
}


void loop() {
  // read from port 1, and see if we have a complete inertialsense packet
  if (Serial1.available()) {
    uint8_t inByte = Serial1.read();

    // This function returns the DID of the message that was just parsed, we can then point the buffer to
    // The right function to handle the message.  We can use a cast to interpret the s_buffer as the
    // kind of message that we received
    uint32_t message_type = is_comm_parse(&comm, inByte);
    switch (message_type)
    {
      case DID_NULL:
        // no valid message yet
        break;
      case DID_GPS_NAV:
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

static void handleINSMessage(ins_1_t* ins)
{
  Serial.print("Lat: ");
  Serial.print(ins->lla[0]);
  Serial.print("\tLon: ");
  Serial.print(ins->lla[1]);
  Serial.print("\tAlt: ");
  Serial.print(ins->lla[2]);
  Serial.print("vel_x: ");
  Serial.print(ins->uvw[0]);
  Serial.print("vel_y: ");
  Serial.print(ins->uvw[1]);
  Serial.print("vel_z: ");
  Serial.print(ins->uvw[2]);
  Serial.print("roll: ");
  Serial.print(ins->theta[0]);
  Serial.print("pitch: ");
  Serial.print(ins->theta[1]);
  Serial.print("yaw: ");
  Serial.println(ins->theta[2]);
}


static void handleGpsMessage(gps_nav_t* gps)
{
  // We only requested the status message.
  // The first 8 bits are the number of sattelites,
  // the second 8 bits are the status flags
  // the last 16 bits are the fix type
  uint8_t num_sattelites = gps->status & GPS_STATUS_NUM_SATS_USED_MASK;
  Serial.print ("num_sats: ");
  Serial.print (num_sattelites, DEC);

  switch (gps->status & GPS_STATUS_FIX_TYPE_MASK)
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
  }
}
