/*

INCLUDE Get-Steering-Angle.

FIRST VERSION to use DAC for STEERING.

MOVE TO v.4 after 19/04 16:00

*/

#include <Servo.h>

int sensorPinX = A8;   
int sensorPinY = A9;  
int sensorPinPotSTEER = A14; 
int sensorPinPot = A15;
int sensorPinBrakeSwitch = 23;
float sensorValueX = 0; 
float sensorValueY = 0; 
int sensorPinS = 0;   
int sensorValueS = 0; 
int sensorValuePot = 0;
int sensorValuePotSTEER = 0;
int steeingMove = 980;
int steeringPosition = 0;
int targetAngleInDegrees = 0;

#include <Wire.h>
#include <Adafruit_MCP4725.h>

//scalar corrector to make sure that Joystick-neutral gives 2.4v 
float correctOutX = 0;
float correctOutY = 0;

//vector corrector make sure that Joystick full sweep gives 0.4 to 4.4v
float correctRatioX = 0.24;
float correctRatioY = 0.24;

int outvalX;
int outvalY;

int vOut;
int vOutSteer;

Adafruit_MCP4725 dac01;
Adafruit_MCP4725 dac02;

void setup() {

  allOut();
  
  Serial.begin (9600);
  Serial.println("Setup");

  dac01.begin(0x60);
  dac02.begin(0x61);

  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel A pin

   pinMode(22, OUTPUT);  //Initiates Relay pin
}

void loop() {
    
    getSteeringAngle();
    
    sensorValueX = analogRead(sensorPinX);
    sensorValueY = analogRead(sensorPinY);
    sensorValueS = analogRead(sensorPinS);

    sensorValueS = digitalRead(23);

    //BRAKE.
    //22 = actual relay actuator
    //23 = random digi pin to light up LED
    if(sensorValueS==1){
      digitalWrite(22, HIGH);
      digitalWrite(24, HIGH);
    }else{
      digitalWrite(22, LOW);
      digitalWrite(24, LOW);
    }



/*
* CHANNEL A OF MOTOR-CONTROL, CURRENTLY UNUSED BUT AVAILABLE.
*
*/     
    digitalWrite(12, HIGH); //Establishes forward direction of Channel A
    digitalWrite(9, LOW);   //Disengage the Brake for Channel A
    analogWrite( 3, sensorValueX );   
/*
* CHANNEL B OF MOTOR-CONTROL FOR STEERING.
*
*/    
    digitalWrite(13, HIGH);  //Establishes backward direction of Channel B
    digitalWrite(8, LOW);   //Disengage the Brake for Channel B

      if( sensorValueY < 250){
      
        if( sensorValueY < 50){
            leftOnly();
            steeingMove=980;//FAST LEFT
        }else{
            steeingMove=920;//CREEP LEFT
            leftOnly();
        }
      }else if ( sensorValueY > 800){
      
        if( sensorValueY > 1050){
            rightOnly();
            steeingMove=830;//FAST RIGHT
      
        }else{
            rightOnly();
            steeingMove=850;//CREEP RIGHT
        }
      }else{
        bothNeutral();
        steeingMove = 890;//STATIC
      }
     steeringPosition = sensorValueY*4;
     if(sensorValueY>3800){
        sensorValueY=3800;
     }
     if(sensorValueY<200){
        sensorValueY=200; 
     }

     
    analogWrite(11, steeingMove ); 
    targetAngleInDegrees = 8;
    steeringPosition = ( ( -80 * targetAngleInDegrees ) + 2360 );

    dac02.setVoltage(steeringPosition, false);
    //dac02.setVoltage(300, false);


    //CONTROL THE MAIN MOTOR USING DAC
    sensorValuePot = analogRead(sensorPinPot);
    vOut = sensorValuePot * 4;

    if (vOut > 4094){
      vOut = 4094;
    }
    
    dac01.setVoltage(vOut, false);

    debug();
   
}


//NEXT CHUNK:
//Show the direction/neutral status of the steering motor
// 41 - yellow LED to indicate Left
// 42 - red LED to indicate Right

void allOut(){
  digitalWrite(41, LOW);
  digitalWrite(43, LOW);
}

void leftOnly(){
  digitalWrite(41, HIGH);
  digitalWrite(43, LOW);
}

void rightOnly(){
  digitalWrite(41, LOW);
  digitalWrite(43, HIGH);
}

void bothNeutral(){
  digitalWrite(41, HIGH);
  digitalWrite(43, HIGH);
}

void debug(){
    //value read from the stick x-axis pot,
    //now, the motor pot
    Serial.print("svX: ");
    Serial.print(sensorValueX);
    Serial.print("\t");

    //value read from the stick y-axis
    Serial.print("svY: ");    
    Serial.print(sensorValueY);
    Serial.print("\t");

    //value of steering movement to the Motor Controller
    //Serial.print("stM: ");    
    //Serial.print(steeingMove);
    //Serial.print("\t");


    //value of steering movement to the Motor Controller
    Serial.print("stP: ");    
    Serial.print(steeringPosition);
    Serial.print("\t");


    Serial.print("tgtA: ");    
    Serial.print(targetAngleInDegrees);
    Serial.print("\t");

    //value of the brake sensor
    Serial.print("BRK: ");    
    Serial.print(sensorValueS);
    Serial.print("\t");

    //value of the motor pot
    Serial.print("svP: ");    
    Serial.print(sensorValuePot);
    Serial.print("\t");
    //value output to the motor DAC
    Serial.print("VOut: ");
    Serial.print(vOut);
    Serial.print("\t");  

     Serial.println("");
}

void getSteeringAngle(){
  
  int ang = 0;
  int i=0;
  char buf[4];
  char c;
  
  Wire.requestFrom(0x7C, 4);    // request 4 bytes from slave device #0x7C / #124
  
  //Serial.println( Wire.available() ); 
  
  while (Wire.available()) { // slave may send less than requested
    c = Wire.read(); // receive a byte as character
    buf[i]=c;
    i++;
  }
  ang=atoi(buf);
  //Serial.print("ST-ANG:");

  Serial.println( ang );
  //Serial.print( "\t" );

  return;
}
