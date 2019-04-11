/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(7);  // attaches the servo on pin 9 to the servo object
  //Serial.begin(9600); 
}
int x=34;
int dir=1;
void loop() {
  /*
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  */

  myservo.write(x);

  //while(1);
  //myservo.write(x);
  
  delay(50);
 
  if(x>140){
    dir=0;
  }

  if(x< 0){
    dir=1;
  }

  if(dir == 1){
     x = x + 1;
  }else{
     x = x - 1;  
  }
  
  //Serial.println(x);
}

