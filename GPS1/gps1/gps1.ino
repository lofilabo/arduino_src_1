#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(3, 4); // RX, TX (TX not used)
const int sentenceSize = 80;

char sentence[sentenceSize];

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Serial.println( "Setup" );
}

void loop()
{
  static int i = 0;
  
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
  float fLat_numbers;
  float fLong_numbers;
  
  getField(field, 0);
  if (strcmp(field, "$GPRMC") == 0)
  {
    
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

    Serial.println(fLong_numbers, DEC);
    
    /*
    int j;
    for(j=0; j<20;j++){
      Serial.println(field[j]);
    }
    delay(2000);
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
