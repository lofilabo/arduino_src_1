#define trigPin 12
#define echoPin 13 
int duration, distance; //to measure the distance and time taken 
int ranger;
int totalSubCount;

int dRange( int distance ){
  
  //Serial.println(distance); 

  if(distance < 1){
    //misfire on detector.  Return 100 'overrange' value.
    return 1000; 
  }
  
  if( distance < 10){
      return 1;
  }else if( distance >= 10 && distance < 20 ){
      return 2;
  }else if( distance >= 20 && distance < 30 ){
      return 3;
  }else if( distance >= 30 && distance < 40 ){
      return 4;
  }else if( distance >= 40 && distance < 20 ){
      return 5;
  }else{
      return 1000;  
  }
  
}

void setup() {
    
    Serial.begin (9600); 
    pinMode(11, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT); 
    digitalWrite(11, LOW);
    digitalWrite(6, LOW);
    totalSubCount = 0;
}

// the loop function runs over and over again forever
void loop() {

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    
    ranger = dRange(distance);
    
    if( ranger < 30){
      //Serial.println(ranger); 
      if(ranger < 2){
          totalSubCount++;
          if( totalSubCount>100 ){
            digitalWrite(11, LOW); 
            digitalWrite(6, HIGH); 
            Serial.println( "DETECTED" );
            
          }
      }else{
          digitalWrite(11, HIGH);
          digitalWrite(6, LOW);
          totalSubCount=0; 
      }
    }else{
       digitalWrite(11, HIGH);
       digitalWrite(6, LOW);   
    }

/*      
    digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait for a second
    digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
    delay(500);                       // wait for a second
*/
}
