// the setup function runs once when you press reset or power the board

int d3H,d4H, countr1, countr2, genCountr, stateGlob;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  d3H=0;
  d4H=0;
  countr1 = 0;
  countr2 = 0;
  genCountr = 0;
  stateGlob = 0;
  
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {

  d3H = digitalRead(4);
  d4H = digitalRead(5);

  genCountr++;

  if( d3H == 1 ){
    countr1++;
  }

  if( d4H == 1 ){
    countr2++;
  }

  if (countr1 > 30){
    //set state = high
    // and relay is Actuated;
    setState(1);
  }

  if (countr2 > 30){
    //set state = high
    // and relay is De-Actuated;
    setState(0);
  }

  if ( d3H == 1 & d4H == 1 ){
    genCountr++;
  }

  if( genCountr > 40 ){
    //if there has been no activity for 40 flops,
    //reset both counters and don't change the relay state.
    genCountr = 0;
    countr1 = 0;
    countr2 = 0; 
  }

  Serial.print( "4-5\t" );
  Serial.print( d3H );
  Serial.print("\t");
  Serial.print( d4H );
  Serial.print("\t");
  Serial.print( countr1 );
  Serial.print("\t");
  Serial.print( countr2 );
  Serial.print("\t");
  Serial.print( genCountr );
  Serial.print("\t");
  Serial.print( stateGlob  );
  Serial.print("\n");

  if( stateGlob == 0){
    digitalWrite(LED_BUILTIN, LOW);
  }else{
    digitalWrite(LED_BUILTIN, HIGH);
  }
  
  delay(50);                       // wait for a second

  setRelay();
  

}

void setRelay(){

  //control 2 device control pins 12 & 11.
  //11 is high when the system state is high;
  //12 is low when the system state is high;
  //11 is low when the system state is low;
  //12 is high when the system state is low;
  
  if (stateGlob==0){
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);
  }else{
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
    
  }  
}

void setState(int stateVal){
    countr1 = 0;
    countr2 = 0;
    stateGlob = stateVal;

}
