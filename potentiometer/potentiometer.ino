int sensorPin0 = A0; 
int sensorValue0 = 0;
int sensorPin1 = A1; 
int sensorValue1 = 0;
int sensorPin2 = A2; 
int sensorValue2 = 0;

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
  Serial.print(sensorValue2);
  Serial.print("\t");

  Serial.print("\n");
  delay( 2 );

}
