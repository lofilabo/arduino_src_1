/* RGB module project code
 */
int redpin = 11; //select the pin for the red LED
int greenpin =10; // select the pin for the  blue LED
int bluepin =9;// select the pin for the green LED

int val;

void setup() {
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  Serial.begin(9600);
}

void loop() 
{
for(val=255; val>0; val--)
  {
   analogWrite(11, 255);
   analogWrite(10, 0);
   analogWrite(9, val);
   delay(10); 
  }
for(val=0; val<255; val++)
  {
   analogWrite(11, 255);
   analogWrite(10, val);
   analogWrite(9, 0);
   delay(10); 
  }
 Serial.println(val, DEC);
}
