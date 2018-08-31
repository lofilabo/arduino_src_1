#include <TimerOne.h>
#include <Servo.h>

#define clawPin 11
#define servoPin 12
#define motorPin 13

#define trigPin 53									
#define echoPin 19										// Pins 20 and 21 need special conditions to work, so use 19 instead
#define echo_int 4										// identifier for the interrupt pin
														// INT4 --> Pin 2, identifier 0
														// INT5 --> Pin 3, identifier 1
														// INT0 --> Pin 21, identifier 2
														// INT1 --> Pin 20, identifier 3
														// INT2 --> Pin 19, identifier 4
														// INT3 --> Pin 18, identifier 5

#define TIMER_US 50										// 50 uS timer duration 
#define TICK_COUNTS 2000								// 25 mS worth of timer ticks, keep at least 2000 so that it doesn't give random readings


float const motorSpeedDefault = 88.5;					//92.50 for 9600 and below baud rate, 88.5 for higher baud rates

Servo Servo1;
Servo Servo2;
Servo Servo3;

float motorSpeed = motorSpeedDefault;
float servoAngle = 90;
float clawAngle = 100;


volatile long echo_start = 0;
volatile long echo_end = 0;
volatile long echo_duration = 0;
volatile int trigger_time_count = 0;


const byte numChars = 32;								// Max number of bytes received from the input buffer (absolute max is 64)
char receivedChars[numChars];
char tempChars[numChars];

float floatFromMessage1 = 0.0;
float floatFromMessage2 = 0.0;
float floatFromMessage3 = 0.0;

boolean newData = false;


const long interval = 100; 								// Has to be equal to or lower than the timeout in serial.Serial) Python setup
														// 100 preferable is preferable in order not to interact with serial.read()
unsigned long currentMillis;
unsigned long previousMillis = 0;


boolean contactReset = false;
boolean cleanBuffer = false;

//==========================

void setup() {
	delay(3000);										// Allow the ECU to initialize

    Serial.begin(38400);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
	
	Servo1.attach(motorPin);
    Servo2.attach(servoPin);
    Servo3.attach(clawPin);

    Timer1.initialize(TIMER_US);						// Initiates timers and pin change + external ISR's
    Timer1.attachInterrupt(timerIsr);
    attachInterrupt(echo_int, echo_interrupt, CHANGE);
}

//==========================

void loop() {
    sendData();
  
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        parseData();
        setOutputs();
		
        newData = false;
    }

    checkForCollision();

    Servo1.write(motorSpeed);
    Servo2.write(servoAngle);
    Servo3.write(clawAngle);
}

//==========================

void sendData() {										// Sends data every x seconds interval
    currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

	  
	  //Serial.println(echo_duration / 58);
      Serial.print(motorSpeed);
      Serial.print(":");
      Serial.print(servoAngle);
      Serial.print(":");
      Serial.println(clawAngle);
    }
}

//==========================

void recvWithStartEndMarkers() {						// Reads data byte by byte and stores in a byte array
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    if (cleanBuffer == true) {							// Cleans the input buffer if a collision has happened so that no more signals are sent
        cleanBuffer = false;
               
        while (Serial.available() > 0) {
            Serial.read();
        }
    }
    else {
        while (Serial.available() > 0 && newData == false) {
            rc = Serial.read();

            if (recvInProgress == true) {
				if (rc != endMarker) {
					receivedChars[ndx] = rc;
					ndx++;
                    if (ndx >= numChars) {
						ndx = numChars - 1;
                   }
                }
                else {
					receivedChars[ndx] = '\0';			// Terminate the string
					recvInProgress = false;
					ndx = 0;
					newData = true;
                }
            }
            else if (rc == startMarker) {
                recvInProgress = true;
           }
        }
    }
}

//==========================

void parseData() {										// Parses data from the byte array
    char * strtokIndx;

    strtokIndx = strtok(tempChars,":");
    floatFromMessage1 = atof(strtokIndx);
 
    strtokIndx = strtok(NULL, ":");
    floatFromMessage2 = atof(strtokIndx);

    strtokIndx = strtok(NULL, ":");
    floatFromMessage3 = atof(strtokIndx);
}

//==========================

void setOutputs() {										// Sets motors' and servos' values to the parsed data values
    motorSpeed = floatFromMessage1;
    servoAngle = floatFromMessage2;
    clawAngle = floatFromMessage3;
}

//==========================

void checkForCollision() {								// Checks for collision detected by the ultrasonic sensor

	if (((echo_duration / 58) <= 40) && ((echo_duration / 58) != 0) && (contactReset == false)) {
		motorSpeed = motorSpeedDefault;
		contactReset = true;

		cleanBuffer = true;
    }
    else if ((((echo_duration / 58) >= 50)) && (contactReset == true)) {
		contactReset = false;
    }
}

//==========================

void timerIsr() {										// Interrupt routine that triggers trigPin every x seconds interval
	trigger_pulse();
}

//==========================

void trigger_pulse() {
      static volatile int state = 0;

      if (!(--trigger_time_count)) {					// Count to TICK_COUNTS value, if timeout - initiate trigger pulse
         trigger_time_count = TICK_COUNTS;
         state = 1;
      }
    
      switch(state) {									// State machine handles delivery of trigger pulse
        case 0:
            break;
        
        case 1:
           digitalWrite(trigPin, HIGH);
           state = 2;
           break;
        
        case 2:
        default:      
           digitalWrite(trigPin, LOW);
           state = 0;
           break;
     }
}

//==========================

void echo_interrupt() {									// Test to see whether the echo signal is high or low
	
	switch (digitalRead(echoPin)) {
    case HIGH:
		echo_end = 0;
		echo_start = micros();
		break;

    case LOW:
		echo_end = micros();

		if ((echo_end - echo_start) / 58 <= 400) {
			echo_duration = echo_end - echo_start;
		}
		
		break;
	}
}
