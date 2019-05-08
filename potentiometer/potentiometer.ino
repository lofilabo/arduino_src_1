int sensorPin0 = A0; 
int sensorValue0 = 0;
int sensorPin1 = A1; 
int sensorValue1 = 0;
int sensorPin2 = A2; 
int sensorValue2 = 0;

int sensorPin3 = A3; 
int sensorValue3 = 0;

int sensorPin4 = A4; 
int sensorValue4 = 0;

int sensorPin5 = A5; 
int sensorValue5 = 0;

bool throttleSelectCollective = false;

int countr = 0;
int onoff = 0;


void setup() {

    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial.begin( 9600 );
    while (!Serial)
    {
        // do nothing
    } ;
    Serial.println("I am in setup");  
}

void loop() {

  countr++;
  if(countr>250){
    countr=0;
    if(onoff==1){
      onoff=0;
    }else{
      onoff=1;  
    }
  }
  if(onoff==1){
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }else{
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }
  
  sensorValue0 = analogRead(sensorPin0);
  Serial.print(sensorValue0);
  Serial.print("\t");

  sensorValue1 = analogRead(sensorPin1);
  Serial.print( 1023 - sensorValue1 );
  Serial.print("\t");

  sensorValue2 = analogRead(sensorPin2);
  sensorValue2 = (( sensorValue2-700 )*3)+54;
  if(sensorValue2<=0){ sensorValue2=0;}
  Serial.print(sensorValue2);
  Serial.print("\t");

  sensorValue3 = analogRead(sensorPin3);
  Serial.print((sensorValue3-100)*10);
  Serial.print("\t");

  sensorValue4 = analogRead(sensorPin4);
  Serial.print(sensorValue4*5);
  Serial.print("\t");

  sensorValue5 = analogRead(sensorPin5);
  sensorValue5 = sensorValue5 - 320;
  if(sensorValue5<=0){sensorValue5=0;}
  if(sensorValue5>1000){sensorValue5=1000;} 
  sensorValue5 = sensorValue5 * 2.5;
  Serial.print( (sensorValue5)  );
  Serial.print("\t");

  //Serial.print( throttleSelectCollective );

  Serial.print("\n");
  delay( 2 );

}
