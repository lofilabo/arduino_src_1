/* 

 ***************** Created by BoxElectrónica**********************

 */

#include <Keypad.h>

String displayString = "";

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Define the Keymap
char keys[ROWS][COLS] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'#','0','*','D'}
};

char bank = 'A';
char msg  = '1';
String dstr = "";
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[COLS] = { 2, 3, 4, 5 };
// Connect keypad COL0, COL1, COL2 and COL3 to these Arduino pins.
byte colPins[ROWS] = { 6, 7, 8, 9 }; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



#include "LedControlMS.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
#define NBR_MTX 4 
LedControl lc=LedControl(12,11,10, NBR_MTX);

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

/* we always wait a bit between updates of the display */
String scrollString = "Message C1    ";
//String scrollString= "X X ";
int stringLength=scrollString.length();
char ch0, ch1, ch2, ch3;
int nextCharIndex=0;






#define ledpin 13

void setup()
{
  selectDisplayString();
  setupLEDMatrix();
  setupKeypad();

}

void loop(){
  
  handleKeypad();
  selectDisplayString();
  writeLEDMatrix();
}

void setupKeypad(){
 pinMode(ledpin,OUTPUT);
 digitalWrite(ledpin, HIGH);
 Serial.begin(9600);  
}

void setupLEDMatrix(){

    /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  Serial.begin (9600);
  Serial.println("Setup");
  Serial.println(scrollString);
  Serial.println(stringLength);

  for (int i=0; i< NBR_MTX; i++){
    lc.shutdown(i,false);
  /* Set the brightness to a low value */
    lc.setIntensity(i,1);
  /* and clear the display */
    lc.clearDisplay(i);
  }
  delay(20);
  lc.clearAll();
  ch0= scrollString[0];
  ch1= scrollString[1];
  ch2= scrollString[2];
  ch3= scrollString[3];
  nextCharIndex=4;
}

void writeLEDMatrix(){
  

  sensorValue = analogRead(sensorPin);
  //Serial.println(sensorValue);
  lc.displayChar(0, lc.getCharArrayPosition(ch0));
  lc.displayChar(1, lc.getCharArrayPosition(ch1));
  lc.displayChar(2, lc.getCharArrayPosition(ch2));
  lc.displayChar(3, lc.getCharArrayPosition(ch3));
  ch0=ch1;
  ch1=ch2;
  ch2=ch3;
  ch3=scrollString[nextCharIndex++];
  if (nextCharIndex>=stringLength) nextCharIndex=0;
  delay( sensorValue );
  lc.clearAll();
  delay(0);  
  
}

void handleKeypad(){
 char key = kpd.getKey();
 /*
   if(key){
     if (key == '1'){
      Serial.println('one');
     }else if(key == '2'){
      Serial.println('two');
     }else{
         Serial.println('other');
     }
   }
 */








 
   if(key){
     switch (key)
     {
     case '*':
     digitalWrite(ledpin, LOW);
     break;
     case '#':
     digitalWrite(ledpin, HIGH);
     break;
     
      case '0':
      //Serial.println("ONE");
      break;

      case '1':
      //Serial.println("ONE");
      msg = '1';
      break;

      case '2':
      //Serial.println("ONE");
      msg = '2';
      break;

      case '3':
      //Serial.println("ONE");
      msg = '3';
      break;

      case '4':
      //Serial.println("ONE");
      msg = '4';
      break;

      case '5':
      //Serial.println("ONE");
      msg = '5';
      break;

      case '6':
      //Serial.println("ONE");
      msg = '6';
      break;

      case '7':
      //Serial.println("ONE");
      msg = '7';
      break;

      case '8':
      //Serial.println("ONE");
      msg = '8';
      break;

      case '9':
      //Serial.println("ONE");
      msg = '9';
      break;


      case 'A':
      //Serial.println("ONE");
      bank = 'A';
      break;

      case 'B':
      //Serial.println("ONE");
      bank = 'B';
      break;

      case 'C':
      //Serial.println("ONE");
      bank = 'C';
      break;

      case 'D':
      //Serial.println("ONE");
      bank = 'D';
      break;




     default:
     int i = 1;
     //Serial.println(key);
     }
   }

}


void selectDisplayString(){

    Serial.println(bank);
    Serial.println(msg);

    if(bank == 'A'){
        if (msg == '1'){
          displayString = "Message A1    ";
        }else if (msg == '2'){
          displayString = "Message A2    ";
        }else if (msg == '3'){
          displayString = "Message A3    ";        
        }else if (msg == '4'){
          displayString = "Message A4    ";        
        }else if (msg == '5'){
          displayString = "Message A5    "; 
        }else if (msg == '6'){
          displayString = "Message A6    "; 
        }else if (msg == '7'){
          displayString = "Message A7    "; 
        }else if (msg == '8'){
          displayString = "Message A8    "; 
        }else if (msg == '9'){
          displayString = "Message A9    "; 
        }else if (msg == '0'){
          displayString = "Message A0    "; 
        }
    }else if (bank == 'B'){
        if (msg == '1'){
          displayString = "Message B1    ";
        }else if (msg == '2'){
          displayString = "Message B2    ";
        }else if (msg == '3'){
          displayString = "Message B3    ";        
        }else if (msg == '4'){
          displayString = "Message B4    ";        
        }else if (msg == '5'){
          displayString = "Message B5    "; 
        }else if (msg == '6'){
          displayString = "Message B6    "; 
        }else if (msg == '7'){
          displayString = "Message B7    "; 
        }else if (msg == '8'){
          displayString = "Message B8    "; 
        }else if (msg == '9'){
          displayString = "Message B9    "; 
        }else if (msg == '0'){
          displayString = "Message B0    "; 
        }
      
    }else if (bank == 'C'){
        if (msg == '1'){
          displayString = "Message C1    ";
        }else if (msg == '2'){
          displayString = "Message C2    ";
        }else if (msg == '3'){
          displayString = "Message C3    ";        
        }else if (msg == '4'){
          displayString = "Message C4    ";        
        }else if (msg == '5'){
          displayString = "Message C5    "; 
        }else if (msg == '6'){
          displayString = "Message C6    "; 
        }else if (msg == '7'){
          displayString = "Message C7    "; 
        }else if (msg == '8'){
          displayString = "Message C8    "; 
        }else if (msg == '9'){
          displayString = "Message C9    "; 
        }else if (msg == '0'){
          displayString = "Message C0    "; 
        }
      
    }else if (bank == 'D'){
        if (msg == '1'){
          displayString = "Message D1    ";
        }else if (msg == '2'){
          displayString = "Message D2    ";
        }else if (msg == '3'){
          displayString = "Message D3    ";        
        }else if (msg == '4'){
          displayString = "Message D4    ";        
        }else if (msg == '5'){
          displayString = "Message D5    "; 
        }else if (msg == '6'){
          displayString = "Message D6    "; 
        }else if (msg == '7'){
          displayString = "Message D7    "; 
        }else if (msg == '8'){
          displayString = "Message D8    "; 
        }else if (msg == '9'){
          displayString = "Message D9    "; 
        }else if (msg == '0'){
          displayString = "Message D0    "; 
        }
      
    }
    
    scrollString = displayString;
}
