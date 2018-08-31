int Led=13;//Define LED interface
int buttonpin=3; //Definition of linear Holzer magnetic sensor interface
int val;//Define numeric variable val
void setup()
{
pinMode(Led,OUTPUT);//Define LED as output interface
pinMode(buttonpin,INPUT);//Definition of linear Holzer force sensor for output interface
Serial.begin (9600);
}
void loop(){
  val=digitalRead(buttonpin);//Read the value of the value of the digital interface 3 to val
  if(val==HIGH)//When the linear Holzer magnetic sensor detects the signal, the LED flashes
  {
    digitalWrite(Led,HIGH);
  }
  else
  {
    digitalWrite(Led,LOW);
  }

  Serial.println(analogRead(A8));
}

