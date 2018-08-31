int x;
String str;

void setup(){
  Serial.begin(1200);
  Serial.write("ONE-TIME-ONLY\n");
}

void loop() 
{
    if(Serial.available() > 0)
    {
        str = Serial.readStringUntil('<');
        x = Serial.parseInt();
        Serial.print(str);
    }
}
