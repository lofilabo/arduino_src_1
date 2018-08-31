#include <Servo.h>

//SPEED CONTROL for RV
//  70 = max rev
//  80 = slow rev
// 100 = brake (lower)
// 105 = brake (upper)
// 106 = slow fwd
// 112 = max fwd

Servo servo1; 
int svrspeed;

void setup() {

  pinMode(1,OUTPUT);
  servo1.attach(12); //analog pin 0
 
  Serial.begin(1200);
  Serial.println("Ready");
  svrspeed = 100;
  servo1.write(svrspeed);
  delay(2000);

   svrspeed = 112;
}

void loop() {

      Serial.println(svrspeed);
      servo1.write(svrspeed);
      delay(200);
      //svrspeed = svrspeed+1;
 

} 

