
#include <Wire.h>

void setup() {
  Wire.begin(0x6C);                // join i2c bus with address #109/0x6D
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write("hello                          <"); // respond with message of 32  bytes
  // as expected by master
}
