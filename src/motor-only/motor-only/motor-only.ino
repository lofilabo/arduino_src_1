#include <Servo.h>
#define motorPin 13

Servo Servo1;

void setup() {


  // put your setup code here, to run once:
    Servo1.attach(motorPin);
        Servo1.write(88.5);
            delay(3000);                    // Allow the ECU to initialize
}

void loop() {
  // put your main code here, to run repeatedly:
    Servo1.write(70);
}
