int count=0;
String str1, str2, str3, allStr;

void setup(){
  Serial.begin(9600);
  Serial.write("ONE-TIME-ONLY\n");
  //Serial1.begin(9600);
  //Serial1.write("ONE-TIME-ONLY\n");
  Serial2.begin(9600);
  Serial2.write("ONE-TIME-ONLY\n");
}

void loop(){
  allStr = "";
   Serial1.write(45); // send a byte with the value 45
   Serial2.write(45); // send a byte with the value 45
   str1 = "EVERY 1 SECOND: " ;
   str2 = String(count);
   str3 =  "\n";
   allStr.concat(str1);
   allStr.concat(str2);
   allStr.concat(str3);
   int bytesSent = Serial.print( allStr );
   //int bytesSent1 = Serial1.print( allStr );
   int bytesSent2 = Serial2.print( allStr );
   //int bytesSent = Serial.write( count );
   delay(250);
   count++;
   Serial.flush();
   //Serial1.flush();
   Serial2.flush();
}
