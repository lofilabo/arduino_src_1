
#include <Servo.h>

Servo myservo13;
Servo myservo12;
Servo myservo11;
Servo myservo10;
Servo myservo09;
Servo myservo08;

// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int dir = 1;
void setup() {

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  
  myservo13.attach(13); 
  myservo12.attach(12); 
  myservo11.attach(11); 
  myservo10.attach(10); 
  myservo09.attach(9); 
  myservo08.attach(8); 
  //Serial.begin(9600); 


  
}

void loop() {
 
  myservo13.write( pos );
  myservo12.write( pos );
  myservo11.write( pos );
  myservo10.write( pos );
  myservo09.write( pos );
  myservo08.write( pos );
  
  if(pos > 179){
    dir=0;
  }

  if(pos<1){
    dir=1;
  }

  if(dir==1){
    pos++;
  }else{
    pos--;
  }
  delay(10);
    
}

