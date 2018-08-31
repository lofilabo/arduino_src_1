#include <ESC.h>
#include <Servo.h>

Servo myservo13; 
int i=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  pinMode(13, OUTPUT);
  myservo13.attach(13); 
}

void loop() {
     
  // put your main code here, to run repeatedly:
  myservo13.write( 100 );
  //delay(100); 
  //i++;
  Serial.println(i);
}
