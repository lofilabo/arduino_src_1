#include <Wire.h>

int countr;
int sensorPin = A0; 
int sensorValue = 0;
int gearAngle = 0;
volatile int pwm_value = 0;
volatile int prev_time = 0;

int targetAngle = -90;


int currentDutyCycle = 1500;
int speedLinear = 0;
int forwardReverse = 1;//1=forward 0=reverse

int getAngleFromSensorInput( int sensorinput ){
  //sensorinput assumes 3.3v, not 5v.
  //THIS IS IMPORTANT. REALLY IMPORTANT.
  //REPEAT.
  //THE SUPPLY VOLTAGE TO THE POT. DIFF. IS 3.3v.
  return ( -1 * ( ( sensorinput/2 ) - 180 ));
}

bool withinZoneOfTollerance(){

    int withinUpperLimit = 0;
    int withinLowerLimit = 0;

    int upperLimit = targetAngle + 10;
    int lowerLimit = targetAngle - 10;

    if (gearAngle < upperLimit){
      withinUpperLimit = 1;
    }

    if (gearAngle > lowerLimit){
      withinLowerLimit = 1;
    }

    if( (withinUpperLimit==1) & (withinLowerLimit==1) ){
      return true;
    }else{
      return false;
    }
    
}
void setup() {

    Serial.begin(9600);

    attachInterrupt(0, rising, RISING);
    
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);

    digitalWrite( 9, HIGH );
    digitalWrite( 8, HIGH );
    digitalWrite( 7, LOW );


    
    countr = 0;
}

void loop() {

  sensorValue = analogRead(sensorPin);
  gearAngle = getAngleFromSensorInput( sensorValue );
  dutyCycleToOperation();
  
        
  //Serial.print( sensorValue );
  //Serial.print( "\t");
  Serial.println( gearAngle );
  delay(100);


/*
 * SENSOR VALUE vs. DEGREES
 * 
 *    -90 deg  = 500
 *    -60 deg  = 440
 *    -30 deg  = 370
 *      0 deg  = 300
 *     30 deg  = 250
 *     60 deg  = 180
 *     90 deg  = 130  
 *     
 */


    //Serial.println( withinZoneOfTollerance() );
    if ( withinZoneOfTollerance() == true ){
         digitalWrite( 7, LOW );
         
    }else{
          if( gearAngle > 140 | gearAngle < -140 ){
            digitalWrite( 7, LOW);
          }else{
              if( targetAngle < gearAngle ){
                  digitalWrite( 7, HIGH );
                  // Anticlockwise.  Angle value DECREASES
                  digitalWrite(8, LOW);
                  digitalWrite(9, LOW);  
                  Serial.println("FORWARD");
                }else if( targetAngle > gearAngle ){
                  digitalWrite( 7, HIGH );    
                  // Clockwise. Angle value INCREASES
                  digitalWrite(8, HIGH);
                  digitalWrite(9, HIGH);  
                  Serial.println("REVERSE");  
              }
          }
    }
  


}

void dutyCycleToOperation(){

    //Serial.print(currentDutyCycle);
    //Serial.print("  -  ");
    if(currentDutyCycle < 1000){
      currentDutyCycle = 1000;
     }    
    
    if(currentDutyCycle > 2000){
      currentDutyCycle = 2000;
     }

    if( currentDutyCycle < 1500 ){
      forwardReverse = 1;
      digitalWrite(13, HIGH); 
    }else{
      forwardReverse = -1;
      digitalWrite(13, LOW); 
    }


      if( currentDutyCycle < 1500){
            speedLinear = ( 1400 - currentDutyCycle )/3.8;
      }else{
            speedLinear = ( currentDutyCycle - 1600 )/1.7;
      }
    

    Serial.print(forwardReverse);
    Serial.print("  -  ");
    Serial.println(speedLinear);
    //delay(10);
    targetAngle = (forwardReverse * speedLinear);    
}


  void rising() {
    attachInterrupt(0, falling, FALLING);
    prev_time = micros();
  }
   
  void falling() {
    attachInterrupt(0, rising, RISING);
    currentDutyCycle = micros()-prev_time;
    
    //Serial.println(currentDutyCycle);
  }
