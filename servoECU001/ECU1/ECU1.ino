
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
  //Serial.begin(9600); 
}

void loop() {
 
  //6 = REV
  //myservo.write( 5 );
 
  //7 = STOP
  //myservo.write( 7 );
  
  //12 = CREEP FWD
  //myservo.write( 12 );

  //14 = FWD
  myservo.write( 14 );
  
}

