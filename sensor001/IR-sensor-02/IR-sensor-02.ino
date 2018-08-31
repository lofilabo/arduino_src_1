     
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
Servo myECU;  // create servo object to control a servo/ECU
int analogPinLL = 1;     
int analogPinL = 2;     
int analogPinC = 3;     
int analogPinR = 4;     
int analogPinRR = 5;     

int analogueAvr = 0;
int threshold = 0;
int valLL = 0;                 
int valL = 0;                 
int valC = 0;                 
int valR = 0;                 
int valRR = 0;                 

int x=34;

void unlightAll(){
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}
void light8(){
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}
void light89(){
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
}
void light9(){
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
}
void light910(){
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
}
void light10(){
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
}

void setup()
{
  Serial.begin(9600);      
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  unlightAll();  
  myservo.attach(7); 
  myservo.write(x);  
  myECU.attach(6);
}

void loop()
{
  //myservo.write( x ); 
  //for(;;){}
  myECU.write( 14 );
  
  valLL = analogRead(analogPinLL);    
  valL = analogRead(analogPinL);    
  valC = analogRead(analogPinC);    
  valR = analogRead(analogPinR);    
  valRR = analogRead(analogPinRR);    
  analogueAvr = (valLL + valL + valC + valR + valRR) / 5;
  threshold = analogueAvr - 200;
  Serial.print(analogueAvr);
  Serial.print("--|||--");
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
  
  Serial.println("");     

//BLOCK 1. Initial Test.

//  if ( valLL > 200 || valL > 200 || valC > 200 || valR > 200 || valRR > 200 ){
  if ( valLL < threshold || valL < threshold || valC < threshold || valR < threshold || valRR < threshold ){
      if (valLL < threshold ){
        Serial.println("FAR LEFT DETECTED");
        light8();     
        myservo.write(x - 8);
      }
    
      if (valL < threshold ){
        Serial.println("LEFT DETECTED");
        light89();     
        myservo.write(x - 4);
      }
    
      if (valC < threshold ){
        Serial.println("CENTRE DETECTED");     
        light9();
        myservo.write(x);
      }
      
      if (valR < threshold ){
        Serial.println("RIGHT DETECTED");   
        light910();  
        myservo.write(x + 4 );
      }
      
      if (valRR < threshold ){
        Serial.println("FARRIGHT DETECTED");   
        light10();  
        myservo.write(x + 8 );
      }
  }else{
       unlightAll();   
        myservo.write( x ); 
  }
  delay(100);
}
