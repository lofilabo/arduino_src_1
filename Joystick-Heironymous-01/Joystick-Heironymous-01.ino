/*
 * The Original One!
 * April 20th 2019 - began combining [potentiometer] code into main program.
 * 
 */

#include <Joystick.h>


int sensorPin0 = A0; 
int sensorValue0 = 0;
int sensorPin1 = A1; 
int sensorValue1 = 0;
int sensorPin2 = A2; 
int sensorValue2 = 0;
int sensorPin3 = A3; 
int sensorValue3 = 0;
int sensorPin4 = A4; 
int sensorValue4 = 0;
int sensorPin5 = A5; 
int sensorValue5 = 0;



// Create the Joystick
Joystick_ Joystick;

// Constant that maps the phyical pin to the joystick button.
const int pinToButtonMap = 9;

void setup() {
  // Initialize Button Pins
  pinMode(pinToButtonMap, INPUT_PULLUP);

  // Initialize Joystick Library


  Joystick.setXAxisRange(-127, 127);
  Joystick.setYAxisRange(-127, 127);
  Joystick.setZAxisRange(-127, 127);
  Joystick.setThrottleRange(0, 255);
  Joystick.setRudderRange(0, 255);  
  
  
  Joystick.begin();
}

// Last state of the button
int lastButtonState = 0;
int cs = 0;
int updown=1;

void loop() {

  // Read pin values
  int currentButtonState = !digitalRead(pinToButtonMap);
  if (currentButtonState != lastButtonState)
  {
    Joystick.setButton(0, currentButtonState);
    lastButtonState = currentButtonState;
  }

  
  testXYAxis(cs);

  if (cs>120){
    updown=0;  
  }else if(cs< -120){
    updown=1;  
  }

  if(updown==1){
    cs+=1;
  }else{
    cs-=1;  
  }
  delay(100);
}



void testXYAxis(unsigned int currentStep)
{
  int xAxis;
  int yAxis;
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);

  xAxis = -1 * (( sensorValue0 - 512) / 4 );
  yAxis = -1 * (( sensorValue1 - 512) / 4 );

    Joystick.setXAxis(xAxis);
    Joystick.setYAxis(yAxis);
    
    Joystick.setThrottle(sensorValue0/4);
    Joystick.setRudder(sensorValue1/4);
}
