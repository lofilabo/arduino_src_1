/************************************************************************

  Test of the Pmod

*************************************************************************

  Description: Pmod_CMPS2
  Calculates the heading of the device form the X and Y components of the magnetic field

  Wiring
  Module<----------> Arduino
  VCC     to        3V3
  GND     to        GND
  SCL     to        A5 (SCL)
  SDA     to        A4 (SDA)

*************************************************************************/

// The earth's magnetic field varies according to its location.
// Add or subtract a constant to get the right value
// of the magnetic field using the following site
// http://www.ngdc.noaa.gov/geomag-web/#declination

#define DECLINATION 0.24 // declination (in degrees) in Saint George (Romania).

/************************************************************************/

#include <Wire.h> //including library for I2C communication

unsigned char CMPS2_address = 0x30; //I2C address of the device

//store highest, middle and lowest values, x component and y component
float Max[2], Mid[2], Min[2], X, Y;

void setup() {
  Serial.begin(9600); //serial initialization
  pinMode(13, OUTPUT);
  delay(10);
  CMPS2_init(); //initialize the compass
}

void loop() {
  delay(500);

  //retrieving and displaying the heading of the compass
  float measured_angle = CMPS2_getHeading();
  Serial.print("Heading = ");
  Serial.print(measured_angle);
  Serial.print("°");
  Serial.print('\t');

  CMPS2_decodeHeading(measured_angle);  //get direction
}

void CMPS2_decodeHeading(float measured_angle) {
  //decoding heading angle according to datasheet
  if (measured_angle > 337.25 | measured_angle < 22.5) {
    Serial.println("North");
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
    if (measured_angle > 292.5) {
      Serial.println("North-West");
    }
    else if (measured_angle > 247.5) {
      Serial.println("West");
    }
    else if (measured_angle > 202.5) {
      Serial.println("South-West");
    }
    else if (measured_angle > 157.5) {
      Serial.println("South");
    }
    else if (measured_angle > 112.5) {
      Serial.println("South-East");
    }
    else if (measured_angle > 67.5) {
      Serial.println("East");
    }
    else {
      Serial.println("North-East");
    }
  }
}

float CMPS2_getHeading(void) {
  float components[2];
  CMPS2_set(false);   //set the polarity to normal
  CMPS2_read_XYZ();  //read X, Y, Z components of the magnetic field
  components[0] = X;  //save current results
  components[1] = Y;
  CMPS2_set(true);   //set the polarity to normal
  CMPS2_read_XYZ();  //read X, Y, Z components of the magnetic field

  //eliminate offset from all components
  components[0] = (components[0] - X) / 2.0;
  components[1] = (components[1] - Y) / 2.0;

  //variables for storing partial results
  float temp0 = 0;
  float temp1 = 0;
  //and for storing the final result
  float deg = 0;

  //calculate heading from components of the magnetic field
  //the formula is different in each quadrant
  if (components[0] < Mid[0])
  {
    if (components[1] > Mid[1])
    {
      //Quadrant 1
      temp0 = components[1] - Mid[1];
      temp1 = Mid[0] - components[0];
      deg = 90 - atan(temp0 / temp1) * (180 / 3.14159);
    }
    else
    {
      //Quadrant 2
      temp0 = Mid[1] - components[1];
      temp1 = Mid[0] - components[0];
      deg = 90 + atan(temp0 / temp1) * (180 / 3.14159);
    }
  }
  else {
    if (components[1] < Mid[1])
    {
      //Quadrant 3
      temp0 = Mid[1] - components[1];
      temp1 = components[0] - Mid[0];
      deg = 270 - atan(temp0 / temp1) * (180 / 3.14159);
    }
    else
    {
      //Quadrant 4
      temp0 = components[1] - Mid[1];
      temp1 = components[0] - Mid[0];
      deg = 270 + atan(temp0 / temp1) * (180 / 3.14159);
    }
  }

  //correct heading
  deg += DECLINATION;
  if (DECLINATION > 0)
  {
    if (deg > 360) {
      deg -= 360;
    }
  }
  else
  {
    if (deg < 0) {
      deg += 360;
    }
  }

  return deg;
}

//reads measurements in mG
void CMPS2_read_XYZ(void) {
  //command internal control register 0 bit 0 (measure)
  Wire.beginTransmission(CMPS2_address);
  Wire.write(0x07);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(8);

  //wait for measurement to be completed
  bool flag = false;
  while (!flag) {
    //jump to status register
    Wire.beginTransmission(CMPS2_address);
    Wire.write(0x06);
    Wire.endTransmission();

    //read its value
    Wire.requestFrom(CMPS2_address, (uint8_t)1);
    int temporal = 0;
    if (Wire.available()) {
      temporal = Wire.read();
    }

    //if the last bit is 1, data is ready
    temporal &= 1;
    if (temporal != 0) {
      flag = true;
    }
  }

  //move address pointer to first address
  Wire.beginTransmission(CMPS2_address);
  Wire.write(0x00);
  Wire.endTransmission();

  //save data
  Wire.requestFrom(CMPS2_address, (uint8_t)6);
  byte tmp[6] = {0, 0, 0, 0, 0, 0}; //array for raw data
  if (Wire.available()) {
    for (int i = 0; i < 6; i++) {
      tmp[i] = Wire.read(); //save it
    }
  }

  //initialize array for data
  float measured_data[2];

  //reconstruct raw data
  measured_data[0] = 1.0 * (int)(tmp[1] << 8 | tmp[0]); //x
  measured_data[1] = 1.0 * (int)(tmp[3] << 8 | tmp[2]); //y

  //convert raw data to mG
  for (int i = 0; i < 2; i++) {
    measured_data[i] = 0.48828125 * (float)measured_data[i];
  }

  X = measured_data[0];
  Y = measured_data[1];

  //correct minimum, mid and maximum values
  if (measured_data[0] > Max[0]) { //x max
    Max[0] = measured_data[0];
  }
  if (measured_data[0] < Min[0]) { //x min
    Min[0] = measured_data[0];
  }
  if (measured_data[1] > Max[1]) { //y max
    Max[1] = measured_data[1];
  }
  if (measured_data[1] < Min[1]) { //y min
    Min[1] = measured_data[1];
  }
  for (int i = 0; i < 2; i++) { //mid
    Mid[i] = (Max[i] + Min[i]) / 2;
  }

  return;
}

//initialize the compass
void CMPS2_init(void) {
  Wire.begin(); // initialization of I2C bus

  //command internal control register 0 for set operation
  Wire.beginTransmission(CMPS2_address);
  Wire.write(0x07);
  Wire.write(0x20);
  Wire.endTransmission();
  delay(10);

  //command internal control register 1 to 16 bit resolution, 8ms measurement time
  Wire.beginTransmission(CMPS2_address);
  Wire.write(0x08);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(10);

  //initialize minimum, mid and maximum values
  for (int i = 0; i < 2; i++) {
    Max[i] = -32768;  //smallest int on 16 bits
    Min[i] = 32767;  //largest int on 16 bits
    Mid[i] = 0;
  }
}

//sets/resets the sensor, changing the magnetic polarity of the sensing element
void CMPS2_set(bool reset) {
  //command internal control register 0 bit 7 (capacitor recharge)
  Wire.beginTransmission(CMPS2_address);
  Wire.write(0x07);
  Wire.write(0x80);
  Wire.endTransmission();
  delay(50);

  if (reset) {
    //command internal control register 0 bit 6 (reset)
    Wire.beginTransmission(CMPS2_address);
    Wire.write(0x07);
    Wire.write(0x40);
    Wire.endTransmission();
    delay(10);
  }
  else {
    //command internal control register 0 bit 5 (set)
    Wire.beginTransmission(CMPS2_address);
    Wire.write(0x07);
    Wire.write(0x20);
    Wire.endTransmission();
    delay(10);
  }
  return;
}
