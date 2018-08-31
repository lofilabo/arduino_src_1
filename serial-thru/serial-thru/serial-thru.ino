void setup() {
  // initialize both serial ports:
  Serial.begin(500000);
  Serial1.begin(500000);
  Serial2.begin(9600);
  Serial.println("here");
  Serial.println(Serial.available());
  Serial.println(Serial1.available());
  Serial.println(Serial2.available());
}

void loop() {
  Serial2.write(5);
  // read from port 1, send to port 0:
  if (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write(inByte);
    Serial2.write(inByte);
  }

  // read from port 0, send to port 1:
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial1.write(inByte);
    Serial2.write(inByte);
  }
}
