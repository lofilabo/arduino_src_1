#include <SoftwareSerial.h>

#include "Wire.h" // For I2C
#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library*
//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the default I2C bus address of the backpack-see article


SoftwareSerial gpsSerial(3, 4); // RX, TX (TX not used)
const int sentenceSize = 80;

char sentence[sentenceSize];

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);
  //Serial.println( "Setup" );

   // Set off LCD module
   lcd.begin (20,4); // 16 x 2 LCD module
   lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
   lcd.setBacklight(HIGH);
   
//   lcd.setCursor(0,1);   
//   lcd.print("Hello, World!");
   
  
}

void loop()
{
  static int i = 0;
  
  if (gpsSerial.available())
  {
    char ch = gpsSerial.read();
    
    if (ch != '\n' && i < sentenceSize)
    {
      sentence[i] = ch;
      i++;
    }
    else
    {
      
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
    
    Serial.print(fLat_numbers, DEC);
    Serial.print("\t");
     char result1[20];
     dtostrf(fLat_numbers, 15, 10, result1); // Leave room for too large numbers!
     lcd.setCursor(0,0);
     lcd.print(result1);    

    
    
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

    Serial.print(fLong_numbers, DEC);
    Serial.print("\t\t");
     char result2[20];
     dtostrf(fLong_numbers, 15, 10, result2); // Leave room for too large numbers!
     lcd.setCursor(0,1);
     lcd.print(result2);    
    /*
    int j;
    for(j=0; j<20;j++){
      Serial.println(field[j]);
    }
    delay(2000);
    */

    Serial.print(sTime);  
    Serial.print("\t");
    Serial.print(sSpeed);  
    Serial.print("\t");
    Serial.print(sCourse);  
    Serial.print("\n");
    
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

     lcd.setCursor(16,0);
     lcd.print("N");
     lcd.setCursor(16,1);
     lcd.print("E");
    
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
