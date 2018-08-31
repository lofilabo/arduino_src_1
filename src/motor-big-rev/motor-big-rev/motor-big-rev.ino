int countr;
int sensorPin = A0; 
int sensorValue = 0;
int gearAngle = 0;
int targetAngle = 0;

int getAngleFromSensorInput( int sensorinput ){
  //sensorinput assumes 3.3v, not 5v.
  //THIS IS IMPORTANT. REALLY IMPORTANT.
  //REPEAT.
  //THE SUPPLY VOLTAGE TO THE POT. DIFF. IS 3.3v.
  return ( -1 * ( ( sensorinput/2 ) - 177 ));
}

bool withinZoneOfTollerance(){

    int withinUpperLimit = 0;
    int withinLowerLimit = 0;

    int upperLimit = targetAngle + 5;
    int lowerLimit = targetAngle - 5;

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
    
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(11, OUTPUT);

    digitalWrite( 13, HIGH );
    digitalWrite( 12, HIGH );
    digitalWrite( 11, LOW );


    
    countr = 0;
}

void loop() {

  sensorValue = analogRead(sensorPin);
  gearAngle = getAngleFromSensorInput( sensorValue );
  Serial.print( sensorValue );
  Serial.print( "\t");
  Serial.println( gearAngle );
  //delay(1000);

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


Serial.println( withinZoneOfTollerance() );
    if ( withinZoneOfTollerance() == true ){
         digitalWrite( 11, LOW );
         
    }else{
          
          if( targetAngle < gearAngle ){
              digitalWrite( 11, HIGH );
              // Anticlockwise.  Angle value DECREASES
              digitalWrite(13, LOW);
              digitalWrite(12, HIGH);  
            }else if( targetAngle > gearAngle ){
              digitalWrite( 11, HIGH );    
              // Clockwise. Angle value INCREASES
              digitalWrite(13, HIGH);
              digitalWrite(12, LOW);    
          }
    }

}


