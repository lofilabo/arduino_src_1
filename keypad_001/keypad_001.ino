/* 

 ***************** Created by BoxElectrónica**********************

 */

#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Define the Keymap
char keys[ROWS][COLS] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'#','0','*','D'}
};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[COLS] = { 2, 3, 4, 5 };
// Connect keypad COL0, COL1, COL2 and COL3 to these Arduino pins.
byte colPins[ROWS] = { 6, 7, 8, 9 }; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define ledpin 13

void setup()
{
 pinMode(ledpin,OUTPUT);
 digitalWrite(ledpin, HIGH);
 Serial.begin(9600);
}

void loop()
{
 char key = kpd.getKey();
 if(key) // Check for a valid key.
 {
 switch (key)
 {
 case '*':
 digitalWrite(ledpin, LOW);
 break;
 case '#':
 digitalWrite(ledpin, HIGH);
 break;
 default:
 Serial.println(key);
 }
 }
}
