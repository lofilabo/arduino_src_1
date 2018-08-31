#include <Servo.h>

//STEERING CONTROL for RV


Servo servo1; 
Servo servo2; 
Servo servo3; 
 
int svrspeed;
int drvspeed;
int dir;
int countr;

void setup() {
  dir = 0;
  countr=0;
  pinMode(1,OUTPUT);
  servo1.attach(13); 
  servo2.attach(12);
  servo3.attach(11);
  
 
  Serial.begin(1200);
  Serial.println("Ready");
  svrspeed = 0;
  drvspeed = 100;
  servo2.write(drvspeed);

  delay(2000);
  drvspeed = 106  ;  
}

void loop() {

  servo2.write( drvspeed );
  static int v = 0;
        Serial.println(svrspeed);
        servo1.write(svrspeed);
        delay(200);
        if( svrspeed > 120  ){
          dir=1;
         
        }

        if( svrspeed < 60  ){
          
          dir=0;
        }
        
        if( dir==0 ){
          svrspeed = svrspeed+5;
        }else{
          svrspeed = svrspeed-5;
        }

        countr++;

        if (countr > 3){
          countr=0;
               if( dir==0 ){
                servo3.write(50);
                
              }else{
                servo3.write(85);
                
              }           
        }

} 

