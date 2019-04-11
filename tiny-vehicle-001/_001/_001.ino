
#include <Servo.h>

int analogPin0 = 0; 
    
int analogPin1 = 1;     
int analogPin2 = 2;     
int analogPin3 = 3;     
int analogPin4 = 4;     
int analogPin5 = 5;     
                      
int val0 = 0; 
          
int val1 = 0;           
int val2 = 0;           
int val3 = 0;           
int val4 = 0;           
int val5 = 0;    

int a = 0;
int t = 0;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

bool standout_low;
int x=34;
int dir=1;

void setup()
{
  myservo.attach(7);
  Serial.begin(9600);  
  pollSensors();
  print_sensor_outputs();       
  delay(500);
}

int determine_standout(){
  //Serial.println(">>>>>>>>>>");
  if(diff(val1) > 10){return 1;}
  if(diff(val2) > 10){return 2;}
  if(diff(val3) > 10){return 3;}
  if(diff(val4) > 10){return 4;}
  if(diff(val5) > 10){return 5;}

  return 0;
  //Serial.println("");
}

int diff(int inval){
  int mdif = inval-a;
  int pdif = sqrt(mdif*mdif);
  //Serial.print(pdif);
  //Serial.print("\t");
  return pdif;
}

void determine_background(){

  t = val1 + val2 + val3 + val4 + val5 ;
  a = t/5;

  if(a > 500){
    //light background  
    standout_low = true;
  }else{
    //dark background
    standout_low = false; 
  }
}

void pollSensors(){
  //val0 = analogRead(analogPin0);    
  val1 = analogRead(analogPin1);    
  val2 = analogRead(analogPin2);    
  val3 = analogRead(analogPin3);    
  val4 = analogRead(analogPin4);    
  val5 = analogRead(analogPin5);    
}

void print_sensor_outputs(){

  //Serial.print(val0);    
  //Serial.print("\t");    
  Serial.print(val1);    
  Serial.print("\t");    
  Serial.print(val2);    
  Serial.print("\t");    
  Serial.print(val3);    
  Serial.print("\t");    
  Serial.print(val4);    
  Serial.print("\t");    
  Serial.print(val5);    
  Serial.print("\t");    
  Serial.print("\n"); 
}

void loop(){
    
    pollSensors();
    //print_sensor_outputs();         
    determine_background();
    int standout = determine_standout();
    Serial.println(standout);

    switch( standout ) {
        case(1):
           myservo.write(150);
          break;
        case(2):
           myservo.write(120);
          break;
        case(3):
           myservo.write(90);
          break;
        case(4):
          myservo.write(60);
          break;
        case(5):
           myservo.write(30);
          break;
        default:
           myservo.write(90);
    }
    
    
   
    

  /*
  delay(100); 
    
  if(x>180){
    dir=0;
  }

  if(x< 0){
    dir=1;
  }

  if(dir == 1){
     x = x + 10;
  }else{
     x = x - 10;  
  }
  */
  Serial.println(x);
  
}
