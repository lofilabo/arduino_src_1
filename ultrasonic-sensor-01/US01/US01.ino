#define trigPin 12
#define echoPin 13

int duration, distance; //to measure the distance and time taken 
int depthOneCounter, depthTwoCounter, lightOn;

void setup() {
        Serial.begin (9600); 
        pinMode(trigPin, OUTPUT); 
        pinMode(echoPin, INPUT);
        pinMode(0, OUTPUT);
        depthOneCounter=0;
        depthTwoCounter=0;
        lightOn = 0;
        digitalWrite(0, LOW);
}

void loop() {
  
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;

    if(distance < 10){
        Serial.println( "DETECTED" );
    }
    
    if (depthOneCounter > 100){
          /*
          Do Things every 5 rotations
          */ 

          if( lightOn == 0 ){
            //Serial.println( "lightOn -> 0" );
            digitalWrite(0, LOW);
            lightOn=1;
          }else{
            //Serial.println( "lightOn -> 1" );
            digitalWrite(0, HIGH);
            lightOn=0;
          }
           
          //Serial.println( "Depth-1" );
          depthOneCounter=0;
          
          if (depthTwoCounter > 5){
            /*
            Do Things once every 25 rotations
            */
            //Serial.println( "Depth-2" );
            depthTwoCounter=0;
          }
          depthTwoCounter++;
    }
    depthOneCounter++;
}
