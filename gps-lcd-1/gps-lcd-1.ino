
#include <SoftwareSerial.h>
#include "TimerObject.h"
#include "Wire.h" // For I2C
/*
#include "LCD.h" // For LCD
*/
#include "LiquidCrystal_I2C.h" // Added library*
//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7
LiquidCrystal_I2C lcd(0x27,20,4); // 0x27 is the default I2C bus address of the backpack-see article

TimerObject *timer1 = new TimerObject(2000); //will call the callback in the interval of 1000 ms


SoftwareSerial gpsSerial(3, 4); // RX, TX (TX not used)
const int sentenceSize = 80;
long miliCount = 0;
long timeSinceLastCheck;
int timeOfNextCheck;
char sentence[sentenceSize];

void FunctionCallback(){
  Serial.println("CALLBACK!!");
}

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);
  // initialize the LCD
  lcd.begin();
  timer1->setOnTimer(&FunctionCallback);
  timer1->Start(); //start the thread.
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("In Setup...");
   
  
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
