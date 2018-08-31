/*

v.4.0.

IN THIS VERSION...
Plan to use getSteeringAngle **AND** getVelocity.


ARCHIVED ATR 11:20, 23/04, passed to ARTHUR for integration.

RICHARD continues with v4.1.
ARTHUR continues with v.5.0

RECOMBINe into v.6.0
*/

#include <Servo.h>

int sensorPinX = A8;   
int sensorPinY = A9;  
int sensorPinPotSTEER = A14; 
int sensorPinPot = A15;
int sensorPinBrakeSwitch = 23;
float sensorValueX = 0; 
float sensorValueY = 0; 
int sensorPinS = 0;   
int sensorValueS = 0; 
int sensorValuePot = 0;
int sensorValuePotSTEER = 0;
int steeingMove = 980;
int steeringPosition = 0;
int targetAngleInDegrees = 0;

int tSeconds = 0;
int tTenthSeconds = 0;
int tHundredthSeconds = 0;
int tThousandthSeconds = 0;

float ang = 0;
float spd = 0;
float mags_x;
float mags_y;
float mags_angle;
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_HMC5883_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

//scalar corrector to make sure that Joystick-neutral gives 2.4v 
float correctOutX = 0;
float correctOutY = 0;

//vector corrector make sure that Joystick full sweep gives 0.4 to 4.4v
float correctRatioX = 0.24;
float correctRatioY = 0.24;

int outvalX;
int outvalY;

int vOut;
int vOutSteer;

Adafruit_MCP4725 dac01;
Adafruit_MCP4725 dac02;

void setup() {

  allOut();
  Serial.begin (9600);
  Serial.println("Setup");
  dac01.begin(0x60);
  dac02.begin(0x61);
  pinMode(22, OUTPUT);  //Initiates Relay pin

 tSeconds = 0;
 tTenthSeconds = 0;
 tHundredthSeconds = 0;
 tThousandthSeconds = 0;
  
}

void loop() {
    
    getSteeringAngleAndSpeed();
    getMagOutput();
    sensorValueS = digitalRead(23);


    if(sensorValueS==1){
      brakeOff();
    }else{
      brakeOn();
    }
    
    analogWrite(11, steeingMove ); 
    targetAngleInDegrees = 0;
    
    steeringPosition = ( ( -80 * targetAngleInDegrees ) + 2360 );
    controlSteeringIndicators();
    dac02.setVoltage(steeringPosition, false);

/*
     *   1840 = reverse crawl
     *   1860 = just-about reverse
     *   1900 = neutral
     *   2290 = just-about forward
     *   2300 = forward crawl
*/
    vOut=2300;
    checkAndCorrectVout();
    dac01.setVoltage(vOut, false);
    debug();
    delay(1);
    timingMonitor();
}


void timingMonitor(){
/*
 tSeconds = 0;
 tTenthSeconds = 0;
 tHundredthSeconds = 0;
 tThousandthSeconds = 0;
*/
  if( tThousandthSeconds > 10 ){
    tThousandthSeconds=0;
    tHundredthSeconds++;
  }

  if( tHundredthSeconds > 10 ){
    tHundredthSeconds=0;
    tTenthSeconds++;
  }

  if( tTenthSeconds > 10 ){
    tTenthSeconds=0;
    tSeconds++;
  }
  tThousandthSeconds++;
}

//NEXT CHUNK:
//Show the direction/neutral status of the steering motor
// 41 - yellow LED to indicate Left
// 42 - red LED to indicate Right

void allOut(){
  digitalWrite(41, LOW);
  digitalWrite(43, LOW);
}

void leftOnly(){
  digitalWrite(41, HIGH);
  digitalWrite(43, LOW);
}

void rightOnly(){
  digitalWrite(41, LOW);
  digitalWrite(43, HIGH);
}

void bothNeutral(){
  digitalWrite(41, HIGH);
  digitalWrite(43, HIGH);
}

void debug(){

    //timing

        Serial.print(tSeconds);    
        Serial.print(":");    
        Serial.print(tTenthSeconds);    
        Serial.print(":");    
        Serial.print(tHundredthSeconds);    
        Serial.print(":");    
        Serial.print(tThousandthSeconds);    
        Serial.print("\t\t"); 
  
      //heading from mag sensor
    Serial.print("hAng: ");    
    Serial.print(mags_angle);
    Serial.print("\t");
  /*
    //value read from the stick x-axis pot,
    //now, the motor pot
    Serial.print("svX: ");
    Serial.print(sensorValueX);
    Serial.print("\t");

    //value read from the stick y-axis
    Serial.print("svY: ");    
    Serial.print(sensorValueY);
    Serial.print("\t");

    //value of steering movement to the Motor Controller
    //Serial.print("stM: ");    
    //Serial.print(steeingMove);
    //Serial.print("\t");
  */

   
   

    //value of steering movement to the Motor Controller
    Serial.print("stP: ");    
    Serial.print(steeringPosition);
    Serial.print("\t");


    Serial.print("ang: ");    
    Serial.print(ang);
    Serial.print("\t");

    Serial.print("spd: ");    
    Serial.print(spd);
    Serial.print("\t");

    //value of the brake sensor
    Serial.print("BRK: ");    
    Serial.print(sensorValueS);
    Serial.print("\t");

    //value of the motor pot
    Serial.print("svP: ");    
    Serial.print(sensorValuePot);
    Serial.print("\t");
    //value output to the motor DAC
    Serial.print("VOut: ");
    Serial.print(vOut);
    Serial.print("\t");  

     Serial.println("");
}

void getSteeringAngleAndSpeed(){
  

  int i=0;
  int b=0;
  int8_t buf[2];
  byte c;
  
  Wire.requestFrom(0x7C, 2);    // request 4 bytes from slave device #0x7C / #124
  
  //Serial.println( Wire.available() ); 
  
  while (Wire.available()) { // slave may send less than requested

    c = Wire.read(); // receive a byte as character
    buf[i]= (int8_t)c;
    //Serial.println( buf[i], DEC );

    if(i==0){
      ang=buf[i];
    }
    if(i==1){
      spd=buf[i];
    }  
    
    i++;
    b++;
  
  }
   //Serial.println( "" );
  //Serial.println( buf );
  /*
  ang=atoi(buf);
  //Serial.print("ST-ANG:");

  Serial.println( ang );
  //Serial.print( "\t" );
  */
  
  return;
}



void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);

  //Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");
  //Serial.println("uT");  
  
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  
  delay(500);
}



void getMagOutput(void) 
{

  sensors_event_t event; 
  mag.getEvent(&event);
  
  float magx, magy;
  
  magx = event.magnetic.x;
  magy = event.magnetic.y;

  magx = magx + 0;    //    <------------------ x correction scalar
  magy = magy + 0;    //    <------------------ y correction scalar

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  //Serial.print(magx); Serial.print("\t");
  //Serial.print(magy); Serial.print("\t");
  //Serial.println("");
  //Serial.print("\t");

  float heading = atan2(magy, magx);

  /*
   * Remember to make declination angle relect mounting anle within the ECU.
   * 
   */
  float declinationAngle = -1.74;
  //heading += declinationAngle;
  //Serial.println(heading);
  
  // Correct for when signs are reversed.
  if(heading < 0)
    //heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
    
  // Convert radians to degrees for readability.
  float headingDegrees = (heading * ( (180 + declinationAngle)/M_PI) ) + declinationAngle; 
  
  //Serial.println(headingDegrees);

    mags_x = magx;
    mags_y = magy;
    mags_angle = headingDegrees;
  //Serial.print("Heading (radians): "); Serial.println(heading);
  //Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
  
  //for(;;){}
  //delay(100);
}


    //BRAKE.
    //22 = actual relay actuator
    //23 = random digi pin to light up LED

    
void brakeOff(){
      digitalWrite(22, HIGH);
      digitalWrite(24, HIGH);
}

void brakeOn(){
      digitalWrite(22, LOW);
      digitalWrite(24, LOW);  
}

void controlSteeringIndicators(){
    if(targetAngleInDegrees > 0){
      leftOnly();
    }else if(targetAngleInDegrees < 0){
      rightOnly();
    }else{
       bothNeutral();
    }
}

void checkAndCorrectVout(){
    if (vOut > 2350){
      vOut = 2350;
    }

    if (vOut < 1800){
      vOut = 1800;
    }  
}




    //CONTROL THE MAIN MOTOR USING DAC
    /*  USEFUL Vout values.
     *   1800 = fast reverse
     *   1820 = medium reverse
     *   1840 = reverse crawl
     *   1860 = just-about reverse
     *   1870 = neutral
     *   1880 = neutral
     *   1890 = neutral
     *   1900 = neutral
     *   2000 = neutral
     *   2050 = neutral
     *   2200 = neutral
     *   2280 = neutral
     *   2290 = just-about forward
     *   2300 = forward crawl
     *   2320 = forward slow
     *   2330 = forward brisk
     *   2340 = forward fast
     */
