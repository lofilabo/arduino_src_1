
#include <SoftwareSerial.h>
#include "TimerObject.h"
#include "Wire.h" // For I2C
#include <Adafruit_HMC5883_U.h>
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
/*
#include "LCD.h" // For LCD
*/
#include "LiquidCrystal_I2C.h" // Added library*
//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7
LiquidCrystal_I2C lcd(0x27,20,4); // 0x27 is the default I2C bus address of the backpack-see article

TimerObject *timer1 = new TimerObject(500); //will call the callback in the interval of 1000 ms


SoftwareSerial gpsSerial(3, 4); // RX, TX (TX not used)
const int sentenceSize = 80;
long miliCount = 0;
long timeSinceLastCheck;
int timeOfNextCheck;
char sentence[sentenceSize];


float xv, yv, zv;
float totxv, totyv, totzv;
float pi = 3.14 ;
float headingAngleRadians;
int headingAngleDegrees;
float headingAngleRadiansWithDir;
int headingAngleDegreesWithDir;
int NorS;//N=0, S=1;
int EorW;//E=0, W=1;
String dir = "";
int headingAvCount = 0;
//calibrated_values[3] is the global array where the calibrated data will be placed
//calibrated_values[3]: [0]=Xc, [1]=Yc, [2]=Zc
float calibrated_values[3];  


void FunctionCallback(){
  
      //Serial.println("CALLBACK!!");

      lcd.setCursor(16,0);
      lcd.print("    ");
      lcd.setCursor(16,0);
      lcd.print(headingAngleDegrees);  

      lcd.setCursor(16,1);
      lcd.print("    ");
      lcd.setCursor(16,1);
      lcd.print(dir);  

      /*
      Serial.flush(); 
      Serial.println("HEADINGS:");
      Serial.print(calibrated_values[0]); 
      Serial.print("\t");
      Serial.print(calibrated_values[1]);
      Serial.print("\t");
      Serial.print(calibrated_values[2]);
      Serial.print( "\t" );
      Serial.print(  headingAngleRadiansWithDir );
      Serial.print( "\t" );
      Serial.print(  headingAngleDegreesWithDir );
      Serial.print("\t");
      Serial.print( dir );
      Serial.print( "\n" );
      */ 
      

}

void setup()
{
  totxv=0;
  totyv=0;
  totzv=0;
  Serial.begin(9600);
  gpsSerial.begin(9600);
  // initialize the LCD
  lcd.begin();
  timer1->setOnTimer(&FunctionCallback);
  timer1->Start(); //start the thread.
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("In Setup...");
    
    //Serial.println("About to begin WIRE");
    Wire.begin();  
    //Serial.println("WIRE started.");
    //Serial.println("Starting....");
    
    if(!mag.begin()){
      Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
      while(1);
    }   

    lcd.setCursor(0,0);
    lcd.print("                                                        ");
    
}

void printTerminalMessagesToLCD(){

    while(Serial.available() > 0 ){
    String str = Serial.readString();
    Serial.println(str);
    if(str.length()>5){
        String sercode = str.substring(0,4);
        //Serial.println(sercode);
        if(sercode.equals("ser1")){
            
            lcd.setCursor(0,2);
            lcd.print("                    ");
            lcd.setCursor(0,2);
            lcd.print(str.substring(5));
        }

        if(sercode.equals("ser2")){
            lcd.setCursor(0,3);
            lcd.print("                    ");
            lcd.setCursor(0,3);
            lcd.print(str.substring(5));
        }

    }
  }
  
}

void loop()
{

  getAndShowHeadings();

  static int i = 0;
  timer1->Update();

  printTerminalMessagesToLCD();

  if (gpsSerial.available()){
        char ch = gpsSerial.read();
        
        if (ch != '\n' && i < sentenceSize){
            sentence[i] = ch;
            i++;
        }else{
             sentence[i] = '\0';
             i = 0;
             displayGPS();
             
        }
  }
}



float parseLatitude(String strIn){
    if( strIn.length()>2){
      String degreesPart = strIn.substring( 0, 2 );
      String minutesPart = strIn.substring( 2 );
      float iDegreesPart = degreesPart.toInt();
      float iMinutesPart = 0.000000000;
      iMinutesPart = minutesPart.toFloat();
      iMinutesPart = iMinutesPart/60;
      float allValue = iDegreesPart + iMinutesPart;
      return allValue;
    }else{
      return 0.00;
    }

}

float parseLongitude(String strIn){

    if( strIn.length()>2){
      String degreesPart = strIn.substring( 0, 3 );
      String minutesPart = strIn.substring( 3 );
      float iDegreesPart = degreesPart.toInt();
      float iMinutesPart = 0.000000000;
      iMinutesPart = minutesPart.toFloat();
      iMinutesPart = iMinutesPart/60;
      float allValue = iDegreesPart + iMinutesPart;
      return allValue;
    }else{
      return 0.00;
    }

  
  return 0.00;
}

void displayGPS()
{
  char field[20];
  String lat_numbers;
  String long_numbers;
  String sSpeed, sCourse, sTime, sVar;
  float fLat_numbers;
  float fLong_numbers;

  getField(field, 0);
  if (strcmp(field, "$GPRMC") == 0)
  {
    //time, speed, course
    getField(field, 1);
    sTime = (String)field;
    sTime = sTime.substring(0,6);

    getField(field, 7);
    sSpeed = (String)field;
    
    getField(field, 8);
    sCourse = (String)field;
    sCourse = sCourse.substring(1,7);
    
    getField(field, 10);
    sVar = (String)field;
    sVar = sVar.substring(1,7);
    
    //Serial.print("Lat: ");
    getField(field, 3);  // number
    lat_numbers = (String)field;
    fLat_numbers = parseLatitude(lat_numbers);
    //Serial.print(field);
    getField(field, 4); // N/S
    //Serial.print(field[0]);
    //Serial.print( " --- ");
    if ( field[0]==78){
      //Serial.println( "NORF" );
    }else if ( field[0]==83 ){
      fLat_numbers = fLat_numbers*-1;
      //Serial.println( "SARF" );
    }else{
      return;//No useful E/W data.  Scrap this reading
    }    
    
   

    
    
    //Serial.print(" Long: ");
    getField(field, 5);  // number
    long_numbers = (String)field;
    fLong_numbers = parseLongitude(long_numbers);
    //Serial.print(field);
    
    getField(field, 6);  // E/W
    //Serial.print(field[0]);
    //Serial.print( " --- ");
    if ( field[0]==87){
      //Serial.println( "WESHT" );
      fLong_numbers = fLong_numbers*-1;
    }else if ( field[0]==69 ){
      //Serial.println( "EASHT" );
    }else{
      return;//No useful E/W data.  Scrap this reading
    }
   
    /*
    int j;
    for(j=0; j<20;j++){
      Serial.println(field[j]);
    }
    delay(2000);
    */

    Serial.print("latlongtime");
    Serial.print("\t");
    Serial.print(fLat_numbers, DEC);
    Serial.print("\t");
    Serial.print(fLong_numbers, DEC);
    Serial.print("\t");
    Serial.print(sTime);  
    /*
    Serial.print("\t");
    Serial.print(sSpeed);  
    Serial.print("\t");
    Serial.print(sCourse);  
    */
    Serial.print("\n");
    
     char result1[20];
     dtostrf(fLat_numbers, 15, 10, result1); // Leave room for too large numbers!
     lcd.setCursor(0,0);
     lcd.print(result1);  

     char result2[20];
     dtostrf(fLong_numbers, 15, 10, result2); // Leave room for too large numbers!
     lcd.setCursor(0,1);
     lcd.print(result2);        
    
     lcd.setCursor(0,0);
     lcd.print("N");
     lcd.setCursor(0,1);
     lcd.print("E");     
/*

    lcd.setCursor(0,2);
    lcd.print("T: ");
    lcd.setCursor(3,2);
    lcd.print(sTime);

    lcd.setCursor(10,2);
    lcd.print("S: ");
    lcd.setCursor(13,2);
    lcd.print(sSpeed); 
 
    lcd.setCursor(0,3);
    lcd.print("C: ");
    lcd.setCursor(3,3);
    lcd.print(sCourse);
    
    lcd.setCursor(10,3);
    lcd.print("V: ");
    lcd.setCursor(13,3);
    lcd.print(sVar);
*/
    
  }
}

void getField(char* buffer, int index)
{
  int sentencePos = 0;
  int fieldPos = 0;
  int commaCount = 0;
  while (sentencePos < sentenceSize)
  {
    if (sentence[sentencePos] == ',')
    {
      commaCount ++;
      sentencePos ++;
    }
    if (commaCount == index)
    {
      buffer[fieldPos] = sentence[sentencePos];
      fieldPos ++;
    }
    sentencePos ++;
  }
  buffer[fieldPos] = '\0';
} 



















void transformation(float uncalibrated_values[3])    
{
  //calibration_matrix[3][3] is the transformation matrix
  //replace M11, M12,..,M33 with your transformation matrix data
double calibration_matrix[3][3] =
  {
      {-0.045, -0.503, 0.011},
      {-0.37, -0.051, 0.004},
      {-0.014, 0.247, 0.475}  
  };
  //bias[3] is the bias
  //replace Bx, By, Bz with your bias data
  double bias[3] =
  {
      1.265,
      -0.109,
      -5.408
  };  

  //calculation
  for (int i=0; i<3; ++i) uncalibrated_values[i] = uncalibrated_values[i] - bias[i];
  float result[3] = {0, 0, 0};
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      result[i] += calibration_matrix[i][j] * uncalibrated_values[j];
  for (int i=0; i<3; ++i) calibrated_values[i] = result[i];
}

void getAndShowHeadings(){
  
  float values_from_magnetometer[3];

  headingAvCount++;

  getHeading();

  if(headingAvCount < 10){
      totxv = totxv + xv;
      totyv = totyv + yv;
      totzv = totzv + zv;
      return;
  }else{
    headingAvCount=0;
    xv=totxv/10;
    yv=totyv/10;
    zv=totzv/10;
    totxv=0;
    totyv=0;
    totzv=0;
  }

  values_from_magnetometer[0] = yv;
  values_from_magnetometer[1] = xv;
  values_from_magnetometer[2] = zv;
  //transformation(values_from_magnetometer);

  if(yv == 0.0 || xv == 0.0 || zv == 0.0){
    //Serial.print(  "A ZERO!!" );
    return;
  }
  //headingAngleRadians = atan(calibrated_values[0]/calibrated_values[1] );
  headingAngleRadians = atan( yv/xv );
  headingAngleDegrees = 57.2958*atan( yv/xv );
  /*
  Serial.print(  xv );
  Serial.print( "\t" );
  Serial.print(  yv );
  Serial.print( "\t" );
  Serial.print(  zv );
  Serial.print( "\t" );
  */
  Serial.print(  headingAngleDegrees );
  Serial.print( "\t" );
  Serial.print( "\n" );
    
  //delay(100); 

/*
  byte data[8] = {EorW, headingAngleDegreesWithDir};
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(0x101, 0, 8, data);
  if(sndStat == CAN_OK){
    Serial.print("\t CAN OK");
  } else {
    Serial.print("\t CAN BAD");
  }
    Serial.println();
  delay(1000); 
 */
  
}




 
void getHeading()
{ 
  sensors_event_t event; 
  mag.getEvent(&event);
  xv = event.magnetic.x;
  yv = event.magnetic.y;
  zv = event.magnetic.z;
}
