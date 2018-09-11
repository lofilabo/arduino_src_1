
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <mcp_can.h>
#include <SPI.h>
#include <Servo.h>

#define speed_PWM_Interrupt 0
#define steer_PWM_Interrupt 1

Adafruit_MCP4725 dac; // constructor
Servo svr_steer;

volatile int pwm_value_speed = 0;
volatile int pwm_value_steer = 0;
volatile int prev_time_speed = 0;
volatile int prev_time_steer = 0;

int currentDutyCycleSpeed = 1500;
int currentDutyCycleSteer = 1500;
float speedLinear = 0.00;
int forwardReverse = 1;//1=forward 0=reverse

int thisMagReadingTime=0;
int prevMagReadingTime=0;

int magnetometerAngle;

int RPM;

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

/*
 * MEGA = Pin 49 == Physical 35 (by standard diagram)
 * UNO / NANO = Physical 32 == D2 (by standard diagram)
 * 
 * We could connect to P32/D2 on the Mega, but the standard diagram
 * wants up to bunch all the SPI things nicely together, so let's keep 
 * the interrupt close to MISO/MOSI/SS/SCK.
 */
//#define CAN0_INT 2                              // Uno/Nano
#define CAN0_INT 49                              // Mega

/*
 * MEGA = Pin 53 == Physical 19 (by standard diagram)
 * UNO / NANO = Physical 10 == D14 (by standard diagram)
 * 
 * We could connect to P10/D19 on the Mega, but the standard diagram
 * wants up to bunch all the SPI things nicely together,
 */
//MCP_CAN CAN0(10);     // Set CS to pin 10
MCP_CAN CAN0(53);     // Set CS to pin 10

byte data[8] = {0x99};
int i=0;

#define LED_CAN_IN 23
#define LED_CAN_OUT 25
#define LED2 27
#define LED3 29
#define LED_STEER_RIGHT 31
#define LED_STEER_CENTRE 33
#define LED_STEER_LEFT 35
#define LED_REVERSE 37
#define LED_NEUTRAL 39
#define LED_FORWARD 41

#define SIGNAL_MAG_BUMP 10



void CAN_IN_ON (){
    digitalWrite(LED_CAN_IN,   HIGH);
}

void CAN_IN_OFF (){
    digitalWrite(LED_CAN_IN,   LOW);
}

void CAN_OUT_ON (){
    digitalWrite(LED_CAN_OUT,   HIGH);
}

void CAN_OUT_OFF (){
    digitalWrite(LED_CAN_OUT,   LOW);
}

void indicate_LEFT (){
    digitalWrite(LED_STEER_LEFT,   HIGH);
    digitalWrite(LED_STEER_CENTRE, LOW);
    digitalWrite(LED_STEER_RIGHT,  LOW);
}
void indicate_CENTRE (){
    digitalWrite(LED_STEER_LEFT,   LOW);
    digitalWrite(LED_STEER_CENTRE, HIGH);
    digitalWrite(LED_STEER_RIGHT,  LOW);
}
void indicate_RIGHT (){
    digitalWrite(LED_STEER_LEFT,   LOW);
    digitalWrite(LED_STEER_CENTRE, LOW);
    digitalWrite(LED_STEER_RIGHT,  HIGH);
}




void indicate_NEUTRAL (){
    digitalWrite(LED_REVERSE, LOW);
    digitalWrite(LED_NEUTRAL, HIGH);
    digitalWrite(LED_FORWARD, LOW);
}
void indicate_FORWARD (){
    digitalWrite(LED_REVERSE, LOW);
    digitalWrite(LED_NEUTRAL, LOW);
    digitalWrite(LED_FORWARD, HIGH);
}
void indicate_REVERSE (){
    digitalWrite(LED_REVERSE, HIGH);
    digitalWrite(LED_NEUTRAL, LOW);
    digitalWrite(LED_FORWARD, LOW);
}

void setup() {
    pinMode(12, OUTPUT);

  /*
   * Set up the pins for STATUS LED.
   */
   pinMode( LED_CAN_IN, OUTPUT);  digitalWrite(LED_CAN_IN, LOW);
   pinMode( LED_CAN_OUT, OUTPUT); digitalWrite(LED_CAN_OUT, LOW);
   pinMode( LED2, OUTPUT);        digitalWrite(LED2, LOW);
   pinMode( LED3, OUTPUT);        digitalWrite(LED3, LOW);
   pinMode( LED_STEER_RIGHT,   OUTPUT);    digitalWrite(LED_STEER_RIGHT, LOW);
   pinMode( LED_STEER_CENTRE,  OUTPUT);    digitalWrite(LED_STEER_CENTRE, LOW);
   pinMode( LED_STEER_LEFT,    OUTPUT);    digitalWrite(LED_STEER_LEFT, LOW);
   pinMode( LED_FORWARD, OUTPUT); digitalWrite(LED_FORWARD, LOW);
   pinMode( LED_NEUTRAL, OUTPUT); digitalWrite(LED_NEUTRAL, LOW);
   pinMode( LED_REVERSE, OUTPUT); digitalWrite(LED_REVERSE, LOW);
   pinMode( SIGNAL_MAG_BUMP, OUTPUT ); digitalWrite(SIGNAL_MAG_BUMP, LOW);
    
    Serial.begin(9600);
    svr_steer.attach(11);
    // when pin D2 goes high, call the rising function
    attachInterrupt(speed_PWM_Interrupt, rising_SPEED_PWM, RISING);
    attachInterrupt(steer_PWM_Interrupt, rising_STEER_PWM, RISING);

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK){
    //Serial.println("MCP2515 Initialized Successfully!");
  }else{
    //Serial.println("Error Initializing MCP2515...");
  }
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                     // Configuring pin for /INT input
      
    
    dac.begin(0x62); // The I2C Address: Run the I2C Scanner if you're not sure 
}


void can_operations(){

  
    // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
    byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
    if(sndStat == CAN_OK){
      CAN_OUT_ON();
      //Serial.println("Message Sent Successfully!");
      CAN_OUT_OFF();
    } else {
      //Serial.println("Error Sending Message...");
    }

  
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN_IN_ON();
    
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    /*
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
    */

    if( rxId == 257 ){
      handleMagnetometerInfo( rxBuf[0], rxBuf[1] );
    }else if( rxId == 263){
      RPM = rxBuf[0];
      if(forwardReverse==1){
        RPM = RPM * -1;  
      }
      Serial.print("RPM: ");
      Serial.print(RPM);
      Serial.print("\n");
    }
    
    //Serial.print(msgString);
    //Serial.println(rxId);
    /*
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }
    */
    CAN_IN_OFF();
    Serial.println();
  }
}

void handleMagnetometerInfo( int direction_EW, int value_degrees ){
      
      prevMagReadingTime = millis();
      
      //Serial.print ("Mag Msg - ");
      //Serial.print(rxId);
      //Serial.print(" - ");
      //Serial.print(direction_EW);
      //Serial.print(" - ");
      //Serial.println(value_degrees);

      /*
       * 0 means EAST
       * 1 means WEST.
       */
      if(direction_EW==0){
        magnetometerAngle = value_degrees;
      }else{
        magnetometerAngle = value_degrees * -1;
      }
      Serial.println( magnetometerAngle );
       
}

void dutyCycleToSteering(){

  /*
  TODO:
  Get value from potentiometer, use to define centre-trim value
  */
  int centreTrim = 925;
  
  //Serial.print("  ---  ");  
    //Serial.print(currentDutyCycleSteer);
    //Serial.print("  -  ");  
    int steerLinear=0;

    if( currentDutyCycleSteer < 1500){
          indicate_LEFT();
          steerLinear = ( currentDutyCycleSteer);
    }else{
          indicate_RIGHT();
          steerLinear = ( currentDutyCycleSteer);
    }

    int steerScalar = steerLinear-centreTrim;
    //steerScalar = 180 - (steerScalar/5.55555);
    steerScalar = steerScalar/5.55555;
    //Serial.print( steerScalar );  
    svr_steer.write(steerScalar);
    //Serial.print("\n");
    //delay(500);
}

void __dutyCycleToSteering(){
  //Serial.print("  ---  ");  
    //Serial.print(currentDutyCycleSteer);
    //Serial.print("  -  ");  
    int steerLinear=0;
    if ( currentDutyCycleSteer > 1450 & currentDutyCycleSteer < 1650){
      //Neutral.  Speed = 0;
      indicate_CENTRE();
      steerLinear = 1500;
    }else{
      if( currentDutyCycleSteer < 1500){
            indicate_LEFT();
            steerLinear = ( currentDutyCycleSteer + 50 );
      }else{
            indicate_RIGHT();
            steerLinear = ( currentDutyCycleSteer - 50 );
      }
    }

    int steerScalar = steerLinear-1000;
    steerScalar = 180 - (steerScalar/5.55555);
    //Serial.print( steerScalar );  
    svr_steer.write(steerScalar);
    //Serial.print("\n");
    //delay(500);
}

void dutyCycleToOperation(){

    //Serial.print(currentDutyCycleSpeed);
    //Serial.print("  -  ");
    if(currentDutyCycleSpeed < 1000){
      currentDutyCycleSpeed = 1000;
     }    
    
    if(currentDutyCycleSpeed > 2000){
      currentDutyCycleSpeed = 2000;
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
    if( currentDutyCycleSpeed < 1420 ){
      forwardReverse = 1;
      digitalWrite(12, HIGH); 
    }else{
      forwardReverse = 0;
      digitalWrite(12, LOW); 
    }

    if ( currentDutyCycleSpeed > 1400 & currentDutyCycleSpeed < 1600){
      //Neutral.  Speed = 0;
      indicate_NEUTRAL();
      speedLinear = 0;
    }else{
      if( currentDutyCycleSpeed < 1500){
            indicate_REVERSE();
            speedLinear = ( 1400 - currentDutyCycleSpeed )/2.4;
      }else{
            indicate_FORWARD();
            speedLinear = ( currentDutyCycleSpeed - 1600 ) / 2.16;
      }
    }

    //Serial.print(forwardReverse);
    //Serial.print("  -  ");
    //Serial.print(speedLinear*40, DEC);
    //delay(200);
}

void loop() {

  dutyCycleToOperation();
  dutyCycleToSteering();
  can_operations();
  dac.setVoltage(speedLinear * 40, false);
  //delay(1000);

  checkMagentometer();
  
}

void checkMagentometer(){
  thisMagReadingTime = millis();
  long timeSinceLastMagReading = thisMagReadingTime - prevMagReadingTime;
  if(timeSinceLastMagReading>500){
    bumpMagnetometer();  
  }
}

void bumpMagnetometer(){
  Serial.println("BUMP THE MAGNETOMETER!!");
  digitalWrite(SIGNAL_MAG_BUMP,   HIGH);
  delay(10);
  digitalWrite(SIGNAL_MAG_BUMP,   LOW);
  delay(1800);
}
 
void rising_SPEED_PWM() {
  attachInterrupt( speed_PWM_Interrupt, falling_SPEED_PWM, FALLING );
  prev_time_speed = micros();
}
 
void falling_SPEED_PWM() {
  attachInterrupt( speed_PWM_Interrupt, rising_SPEED_PWM, RISING );
  currentDutyCycleSpeed = micros()-prev_time_speed;
  
  //Serial.print(currentDutyCycleSpeed);
  //Serial.print("\t");
}

void rising_STEER_PWM() {
  attachInterrupt( steer_PWM_Interrupt, falling_STEER_PWM, FALLING );
  prev_time_steer = micros();
}
 
void falling_STEER_PWM() {
  attachInterrupt( steer_PWM_Interrupt, rising_STEER_PWM, RISING );
  currentDutyCycleSteer = micros()-prev_time_steer;
  
  //Serial.println(currentDutyCycleSteer);
}
