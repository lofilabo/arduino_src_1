#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards


const int 
PWM_A   = 3,
DIR_A   = 12,
BRAKE_A = 9,
SNS_A   = A0;


void setup() {
  // Configure the A output
  pinMode(BRAKE_A, OUTPUT);  // Brake pin on channel A
  pinMode(DIR_A, OUTPUT);    // Direction pin on channel A

  // Open Serial communication
  Serial.begin(9600);
  Serial.println("Motor shield DC motor Test:\n");

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600); 
  
}

int x=30;
int dir=1;

void loop() {
 
        myservo.write(x);
        
        digitalWrite(BRAKE_A, LOW);  // setting brake LOW disable motor brake
        digitalWrite(DIR_A, HIGH);   // setting direction to HIGH the motor will spin forward
      
        analogWrite(PWM_A, 255);     // Set the speed of the motor, 255 is the maximum value 
        delay(1000);                 

        myservo.write(x-5);
        delay(1000); 

        myservo.write(x+5);
        delay(1000); 

        myservo.write(x-10);
        delay(1000); 
        
        myservo.write(x+10);
        delay(1000); 
        
        myservo.write(x-15);
        delay(1000); 
        
        myservo.write(x+15);
        delay(1000); 
        

        analogWrite(PWM_A, 0);  

        while(1);

}
