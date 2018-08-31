
#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac; // constructor

volatile int pwm_value = 0;
volatile int prev_time = 0;

int currentDutyCycle = 1500;
int speedLinear = 0;
int forwardReverse = 1;//1=forward 0=reverse



void setup() {
  pinMode(13, OUTPUT);
    Serial.begin(9600);
    // when pin D2 goes high, call the rising function
    attachInterrupt(0, rising, RISING);
    dac.begin(0x62); // The I2C Address: Run the I2C Scanner if you're not sure 
}

void dutyCycleToOperation(){

    Serial.print(currentDutyCycle);
    Serial.print("  -  ");
    if(currentDutyCycle < 1000){
      currentDutyCycle = 1000;
     }    
    
    if(currentDutyCycle > 2000){
      currentDutyCycle = 2000;
     }
    /*
     * VERY SPECIAL CASE.
     * If duty cycle drops below 1000, we assume Something Bad.
     * 1. Set duty cycle to Neutral
     * 2. Set speed to 0
     * 3. Set direction to forward
     */
     /*
    if(currentDutyCycle < 1000){
      currentDutyCycle = 1500;
      forwardReverse = 1;
      speedLinear = 0;
      return;
    }    
    
    if(currentDutyCycle > 2000){
      currentDutyCycle = 1500;
      forwardReverse = 1;
      speedLinear = 0;
      return;
    }
  */
    if( currentDutyCycle < 1500 ){
      forwardReverse = 1;
      digitalWrite(13, HIGH); 
    }else{
      forwardReverse = 0;
      digitalWrite(13, LOW); 
    }

    if ( currentDutyCycle > 1400 & currentDutyCycle < 1600){
      //Neutral.  Speed = 0;
      speedLinear = 0;
    }else{
      if( currentDutyCycle < 1500){
            speedLinear = ( 1400 - currentDutyCycle )/4;
      }else{
            speedLinear = ( currentDutyCycle - 1600 )/4;
      }
    }

    Serial.print(forwardReverse);
    Serial.print("  -  ");
    Serial.println(speedLinear*40);
    delay(200);
}

void loop() {

  dutyCycleToOperation();
  dac.setVoltage(speedLinear * 40, false);
}
 
void rising() {
  attachInterrupt(0, falling, FALLING);
  prev_time = micros();
}
 
void falling() {
  attachInterrupt(0, rising, RISING);
  currentDutyCycle = micros()-prev_time;
  
  //Serial.println(currentDutyCycle);
}
