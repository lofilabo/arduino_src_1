const long micros_per_minute = 60000000.00;

int sensorPin = 2; 
int value = 0; 
int chop = 0;
int sustainhilo = 0;

long pwm_value = 0;
long prev_time = 0;

int avg_counter = 0;
long avg_pwm = 0;
long total_time = 0;

float rpm;

void setup() { 
 Serial.begin(9600); 
} 


void statechange_hi_to_lo(){

  avg_counter++;

    if( avg_counter > 7){
      total_time = micros()-prev_time;
      total_time = total_time / 1;
      rpm = ( micros_per_minute / (float)total_time );
      
      Serial.println(rpm); 
      avg_counter=0;
      /*
      Serial.print("triggered\t");
      Serial.print(  prev_time );
      Serial.print( "\t"  );
      Serial.print(  micros() );
      Serial.print( "\t"  );
      Serial.print( total_time  );
      Serial.print( "\t"  );
      Serial.println( avg_pwm  );
      */
      prev_time = micros();
    }
   
}

void statechange_lo_to_hi(){

}
void loop() { 
 value = analogRead(sensorPin); 

 if(value > 120){
    chop = 1;
     if(sustainhilo == 0){
      statechange_lo_to_hi();
      sustainhilo=1;
    }
 }else{
    chop = 0;
    if(sustainhilo == 1){
      statechange_hi_to_lo();
      sustainhilo=0;
    }   
 }
 //Serial.println(chop); 
 
 //Serial.println(pwm_value);
 delay(0); 
} 

