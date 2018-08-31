int sensorPin1 = A0;    // select the input pin for the potentiometer
int sensorPin2 = A15;    // select the input pin for the potentiometer
int sensorPin3 = A7;    // select the input pin for the potentiometer

float sensorValue1 = 0;  // variable to store the value coming from the sensor
float sensorValue2 = 0;  // variable to store the value coming from the sensor
float sensorValue3 = 0;  // variable to store the value coming from the sensor
float sensiblesensor_i,sensiblesensor_j,sensiblesensor_k;
void setup() {
Serial.begin (9600);
  Serial.println("Setup");
}

void loop() {
  // read the value from the sensor:
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  sensorValue3 = analogRead(sensorPin3);

  sensiblesensor_i = (sensorValue1 * 2)/100;  
  sensiblesensor_j = (sensorValue2 * 2)/100; ;
  sensiblesensor_k = (sensorValue3 * 2)/100; ;
  //Serial.println( sensorValue );
  
  Serial.print( sensiblesensor_i );
  Serial.print( "\t" );
  Serial.print( sensiblesensor_j );
  Serial.println("");

  if(sensiblesensor_k>18){
    Serial.println("BUMP!");
  }

  
}
