#include <Servo.h>

int sensorPinX = A8;   
float sensorValueX = 0; 
int sensorPinY = A9;   
float sensorValueY = 0; 
int sensorPinS = A10;   
int sensorValueS = 0; 

//scalar corrector to make sure that Joystick-neutral gives 2.4v 
float correctOutX = 0;
float correctOutY = 0;

//vector corrector make sure that Joystick full sweep gives 0.4 to 4.4v
float correctRatioX = 0.24;
float correctRatioY = 0.24;

int outvalX;
int outvalY;

void setup() {
  Serial.begin (9600);
  Serial.println("Setup");

  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel A pin

   pinMode(22, OUTPUT);  //Initiates Relay pin
}

void loop() {
  // put your main code here, to run repeatedly:
    sensorValueX = analogRead(sensorPinX);
    sensorValueY = analogRead(sensorPinY);
    sensorValueS = analogRead(sensorPinS);

    if(sensorValueS==0){
      sensorValueS = 1;
      digitalWrite(22, HIGH);
    }else{
      sensorValueS = 0;
      digitalWrite(22, LOW);
    }

    //sensorValueX = sensorValueX*0.001;
    //sensorValueY = sensorValueY*0.001;
    
    Serial.print(sensorValueX);
    Serial.print("\t");
    Serial.print(sensorValueY);
    Serial.print("\t");
    Serial.println(sensorValueS);
    
    digitalWrite(12, HIGH); //Establishes forward direction of Channel A
    digitalWrite(9, LOW);   //Disengage the Brake for Channel A
    //analogWrite(3, 255 );   //Spins the motor on Channel A at full speed
    outvalX = 255 - (sensorValueX * correctRatioX) + correctOutX ;

    if( outvalX>255 ){
      outvalX=255;
    }

    if( outvalX<1 ){
      outvalX=1;
    }
    
    
    analogWrite( 3, outvalX );   //Spins the motor on Channel A at full speed
    
    digitalWrite(13, HIGH);  //Establishes backward direction of Channel B
    digitalWrite(8, LOW);   //Disengage the Brake for Channel B
    //analogWrite(11, 255);    //Spins the motor on Channel B at half speed
    outvalY = 255 - (sensorValueY * correctRatioY) + correctOutY ;

    if( outvalY>255 ){
      outvalY=255;
    }

    if( outvalY<1 ){
      outvalY=1;
    }
    
    analogWrite(11, outvalY );    //Spins the motor on Channel B at half speed
}
