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

bool throttleSelectCollective = false;


void setup() {
    Serial.begin( 9600 );
    while (!Serial)
    {
        // do nothing
    } ;
    Serial.println("I am in setup");  
}

void loop() {
  
  sensorValue0 = analogRead(sensorPin0);
  Serial.print(sensorValue0);
  Serial.print("\t");

  sensorValue1 = analogRead(sensorPin1);
  Serial.print(sensorValue1);
  Serial.print("\t");

  sensorValue2 = analogRead(sensorPin2);
  sensorValue2 = ( sensorValue2-700 )*3;
  if(sensorValue2<=0){
    sensorValue2=0;
    throttleSelectCollective=true;
  }else{
    throttleSelectCollective=false;  
  }
  Serial.print(sensorValue2);
  Serial.print("\t");

  sensorValue3 = analogRead(sensorPin3);
  Serial.print((sensorValue3-100)*10);
  Serial.print("\t");

  sensorValue4 = analogRead(sensorPin4);
  Serial.print(sensorValue4*5);
  Serial.print("\t");

  Serial.print( throttleSelectCollective );

  Serial.print("\n");
  delay( 2 );

}
