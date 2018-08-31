#include <Servo.h>
Servo myservo;  // create servo object to control a servo

int analogPinLL = 1;     
int analogPinL = 2;     
int analogPinC = 3;     
int analogPinR = 4;     
int analogPinRR = 5;     
              
int valLL = 0;                 
int valL = 0;                 
int valC = 0;                 
int valR = 0;                 
int valRR = 0;      


void setup()
{
  Serial.begin(9600);      
  
}


void loop(){

  valLL = analogRead(analogPinLL);    
  valL = analogRead(analogPinL);    
  valC = analogRead(analogPinC);    
  valR = analogRead(analogPinR);    
  valRR = analogRead(analogPinRR);
  
  Serial.print(valLL);
  Serial.print("-|-");
  Serial.print(valL);
  Serial.print("-|-");
  Serial.print(valC);
  Serial.print("-|-");
  Serial.print(valR);
  Serial.print("-|-");
  Serial.print(valRR);
  Serial.println();
  delay(400);  
  
}
