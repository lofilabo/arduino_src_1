const int 
PWM_B   = 11,
DIR_B   = 12,
SNS_B   = A0;

int levelV = 0;
int dirn = 0;

void setup() {
  pinMode(DIR_B, OUTPUT);    // Direction pin on channel A
  Serial.begin(9600);
}

void loop() {

        analogWrite(PWM_B, levelV); 
        //delay(1); 
        Serial.println(analogRead(SNS_B));

        if (dirn==0){
          levelV++;
        }else{
          levelV--; 
        }
            
        if(levelV>240)dirn=1;

        if(levelV<10)dirn=0;
        

}
