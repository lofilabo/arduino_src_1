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

      // Set the outputs to run the motor forward

        myservo.write(x);
        
        digitalWrite(BRAKE_A, LOW);  // setting brake LOW disable motor brake
        digitalWrite(DIR_A, HIGH);   // setting direction to HIGH the motor will spin forward
      
        analogWrite(PWM_A, 240);     // Set the speed of the motor, 255 is the maximum value 
        delay(2000);                 // hold the motor at full speed for 5 seconds

        myservo.write(10);
        delay(2200); 
        myservo.write(x);
        delay(2000);         

        myservo.write(50);
        delay(2200); 
        myservo.write(x);
        delay(2000); 
        
        analogWrite(PWM_A, 0);  

        while(1);
      // Brake the motor
      /*
        Serial.println("Start braking\n");
        // raising the brake pin the motor will stop faster than the stop by inertia
        digitalWrite(BRAKE_A, HIGH);  // raise the brake
        delay(500);
      */
      // Set the outputs to run the motor backward
       /*     
        Serial.println("Backward");
        digitalWrite(BRAKE_A, LOW);  // setting againg the brake LOW to disable motor brake
        digitalWrite(DIR_A, LOW);    // now change the direction to backward setting LOW the DIR_A pin
      
        analogWrite(PWM_A, 50);     // Set the speed of the motor
      
        delay(2000);
        Serial.print("current consumption backward: ");
        Serial.println(analogRead(SNS_A));

        // now stop the motor by inertia, the motor will stop slower than with the brake function
        analogWrite(PWM_A, 0);       // turn off power to the motor
      
        Serial.print("current brake: ");
        Serial.println(analogRead(A0));
        Serial.println("End of the motor shield test with DC motors. Thank you!");
      */

        

}
