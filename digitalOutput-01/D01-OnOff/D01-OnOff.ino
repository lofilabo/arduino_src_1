void setup() {
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
//  digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
//  delay(500);                       // wait for a second
}
