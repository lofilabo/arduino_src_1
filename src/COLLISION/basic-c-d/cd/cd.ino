#include <MAVERICK.h>
float steering=0;
float last_bearing;
float bearing; 
float steerangle;
float current_bearing,pre_bearing,error1,error2;
long i=0;
long j=0;
long k=0;
float x=0;

int obsCount = 0;

void loop() 
{ 
 //scanning(); 
 //receive();
 //follow_compass();

 

 
 k=read_ultrasonic();
 Serial.println(k);
    drive(-0.35);

  if(k<20 && k>5){
    evade();  
  }
 }

void evade(){

    if(obsCount>0){
      
        drive(0);
        for(;;){}   
   }
  
    Serial.println("EVADING");

      
/*      
      drive(0.5);
      delay(2500);
      
      drive(-0.35);
      
      //BEGIN AVOID
      steer(-50);
      delay(4000);
      steer(-3);
      delay(100);
      steer(50);
      delay(2900);

      //MOVE PAST
      steer(-3);
      delay(500);

      //RECOVER
      steer(50);
      delay(2000);
      steer(-3);
      delay(1200);
      steer(-50);
      delay(3200);
*/

      drive(0.5);
      delay(1400);
      
      drive(-0.35);
      
      //BEGIN AVOID
      steer(-50);
      delay(2400);
      steer(50);
      delay(1600);
/*      
      steer(-3);
      delay(100);
      steer(50);
      delay(2800);

      //MOVE PAST
      steer(-3);
      delay(0);

      //RECOVER
      steer(50);
      delay(500);
      steer(-3);
      delay(0);
      steer(-50);
      delay(2000);



*/      
      steer(-3);      
      obsCount++;
}

void receive()
 {

  /*
  if (w.messageReceived())
  {    
    String myString = w.getMessage(); ////for receiving the message
    led_out(1,HIGH);
    String bearing=myString.substring(0,4);
    String steerangle=myString.substring(6,10);
    String ultrasonic=myString.substring(10);
    Serial.println(bearing);
    Serial.println(steerangle);
    Serial.println(ultrasonic);
    //String a=myString.substring(0);
    //float next_bearing=a.toFloat();
  }
  */

  
 }
void scanning()
{
  /*
  steer(-x);
  i=read_ultrasonic();
  clear_LCD();
  Serial3.print(j);
  nextline_LCD();
  Serial3.print(k);
  Serial.println(i);
  if (i<=25)  {  drive(0);    }
  if (i<=30)  
  { 
    drive(0);  
    led_out(2,HIGH);
    turret(0); 
   }
  if (30<i<50) 
  {    
      led_out(2,LOW);
      turret(12+x);//turn left
      delay(100);
      j=read_ultrasonic();//j left
      turret(-12+x);
      delay(100);
      k=read_ultrasonic();//k right
      if (j-k>=2)         {      x=x-10;    turret(x);   steer(x); drive(-0.27);}  //forward 0.27 backward -0.27
      else if (k-j>=2)    {      x=x+10;    turret(x);   steer(x); drive(-0.27);}
    if (x>=60){x==50;}
    if (x<=-60){x==-50;}
   }
 if (45<=i<55) {  drive(0.35); }
 if (i>=50) 
 {      
    drive(-0.35);
    }
 */
}

void setup() {
 initialise();
 w.setMyID('2');  ///set id for slaves
 Serial.begin(9600);
   steer(-3);
  //drive(-0.35);
 clear_LCD();
}


//void follow_compass()
//{
//      if (w.messageReceived())
//  {   
//    current_bearing=read_compass(); 
//    String myString = w.getMessage(); 
//    float next_bearing=myString.toFloat();
//    error1=next_bearing-current_bearing-1.5;
//    error2=current_bearing-last_bearing-2.0;
//    float error3=(error1-error2)/3.5;
//    steering=1.0*error1+0.5*error2+0.10*error3;
//    if (steering>50)          {      steering=50;    }
//    else if (steering<-50)    {      steering=-50;    }
//    else                      {    steering=steering;    }
//    steer(steering); 
//    if (i<=30)  {  drive(0);  }
//    if (30<i<50){ drive(0.27);}
//    if (i>50)   { drive(0.4); }
//  
//    last_bearing=current_bearing;
//    //drive(velocity);
//    Serial.println(myString);
//    Serial.println(steering);
//    //Serial.println(velocity);
//  }
// // else drive(0);   
//}

