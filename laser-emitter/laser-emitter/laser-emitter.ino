/* Laser transmitter module test project
*/
char val;
int i;

void setup()
{
  Serial.begin(9600); 
  pinMode(13, OUTPUT); 
  i=0; 
}
void loop() {

  if(i==0){
    digitalWrite(13, HIGH);  
    i=1;
  }else{
    digitalWrite(13, HIGH);  
    i=0;
  }
  
  
  /*
   val=Serial.read();
   if(val=='1')
  {
     digitalWrite(13, HIGH);  
   }
  if(val=='0')
  {
    digitalWrite(13, LOW);  
   }
   */
    delay(1);
}
