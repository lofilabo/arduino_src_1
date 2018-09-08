#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10);     // Set CS to pin 10


const long micros_per_minute = 60000000.00;

int sensorPin = 0; 
int value = 0; 
int chop = 0;
int sustainhilo = 0;

long pwm_value = 0;
long prev_time = 0;

int avg_counter = 0;
long avg_pwm = 0;
long total_time = 0;

float rpm;
int irpm;

void setup() { 
 Serial.begin(9600); 

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

 
} 


void statechange_hi_to_lo(){
  //Serial.println("Hi-Lo");
  avg_counter++;

    if( avg_counter > 7){
      total_time = micros()-prev_time;
      total_time = total_time / 1;
      rpm = ( micros_per_minute / (float)total_time );
      irpm = (byte)rpm;
      
      Serial.println( irpm ); 
      byte data[8] = {irpm};
      byte sndStat = CAN0.sendMsgBuf(0x107, 0, 8, data);
      if(sndStat == CAN_OK){
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }      
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

 if(value > 140){
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

