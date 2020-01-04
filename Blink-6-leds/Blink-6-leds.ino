int sensorPin0 = A0; 
int sensorValue0 = 0;
int sensorPin1 = A1; 
int sensorValue1 = 0;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  only1(); 
  delay(sensorValue0);

  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  only2(); 
  delay(sensorValue0);
  
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  only3(); 
  delay(sensorValue0);
  
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  only4(); 
  delay(sensorValue1);
  
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  only5(); 
  delay(sensorValue1);
  
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  only6(); 
  delay(sensorValue1);
        
}

void only1(){
  digitalWrite(1, HIGH);  
  digitalWrite(2, LOW);   
  digitalWrite(3, LOW);   
  digitalWrite(4, LOW);   
  digitalWrite(5, LOW);   
  digitalWrite(6, LOW); 
}

void only2(){
  digitalWrite(1, LOW);  
  digitalWrite(2, HIGH);   
  digitalWrite(3, LOW);   
  digitalWrite(4, LOW);   
  digitalWrite(5, LOW);   
  digitalWrite(6, LOW); 
}

void only3(){
  digitalWrite(1, LOW);  
  digitalWrite(2, LOW);   
  digitalWrite(3, HIGH);   
  digitalWrite(4, LOW);   
  digitalWrite(5, LOW);   
  digitalWrite(6, LOW); 
}

void only4(){
  digitalWrite(1, LOW);  
  digitalWrite(2, LOW);   
  digitalWrite(3, LOW);   
  digitalWrite(4, HIGH);   
  digitalWrite(5, LOW);   
  digitalWrite(6, LOW); 
}

void only5(){
  digitalWrite(1, LOW);  
  digitalWrite(2, LOW);   
  digitalWrite(3, LOW);   
  digitalWrite(4, LOW);   
  digitalWrite(5, HIGH);   
  digitalWrite(6, LOW); 
}

void only6(){
  digitalWrite(1, LOW);  
  digitalWrite(2, LOW);   
  digitalWrite(3, LOW);   
  digitalWrite(4, LOW);   
  digitalWrite(5, LOW);   
  digitalWrite(6, HIGH); 
}
