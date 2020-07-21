void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), startcounter_1, RISING);
  attachInterrupt(digitalPinToInterrupt(2), stopcounter_1, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), startcounter_2, RISING);
  attachInterrupt(digitalPinToInterrupt(3), stopcounter_2, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
    
}

void startcounter_1(){
  Serial.print("2-PUSH\n");
}

void startcounter_2(){
  Serial.print("3-PUSH\n");
}

void stopcounter_1(){
  Serial.print("2-RELEASE\n");
}

void stopcounter_2(){
  Serial.print("3-RELEASE\n");
}
