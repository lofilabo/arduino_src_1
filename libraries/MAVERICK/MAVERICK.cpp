/*
Maverick robot library
For the Summer School Robotics
GPS Robotics
Created by Samuel Wane, Harper Adams University, August 2015
Adapted for single folder library Matt Butler April 2016
Updated for wireless summer 2016
*/


#include "MAVERICK.h"

// the inertia unit
FreeSixIMU sixDOF;

//setup pins for Arduino Mega 2560
#define led1 38
#define led2 40
#define led3 42
#define button1 44
#define button2 46
#define button3 48


//set limits and create steering servo object
#define steerservopin 11
#define steer_angle_max 140
#define steer_angle_min 40
VarSpeedServo steerservo;

//make an accelerometer object
ADXL345 accelReader;


//driving wheels control
#define driveservopin 12
VarSpeedServo driveservo;


//set limits and create turret servo object
#define turretservopin 10
#define turret_max 170
#define turret_min 20
VarSpeedServo turretservo;

// Ultrasonic sensor
#define trigpin 51
#define echopin 49

//Compass
#define I2C_Address 0x60   // Defines address of CMPS10

//Functions


String lastMessage; // not happy with this
String lastMessage1;
String lastMessage2;

volatile long unsigned int lastChk = 666;

//Functions for the trailer
Trailerclass::Trailerclass(){}
Trailerclass::~Trailerclass(){}

Trailerclass trailer;

double Trailerclass::trailer_angleclass(){
  double _trailer_offset=135;
  int sensorPin=A8;
  int sensorValue = analogRead(sensorPin); //0-1022
  double sensorAngle=double(sensorValue)/1024*270;
 
  sensorAngle=sensorAngle-_trailer_offset;
  return sensorAngle; 
}
void Trailerclass::set_trailer_centreclass(void)
{
  int sensorPin=A8;
  int sensorValue = analogRead(sensorPin); //0-1022
  double sensorAngle=double(sensorValue)/1024*270;
  _trailer_offset=sensorAngle;
}

void set_trailer_centre(void)
{
	trailer.set_trailer_centreclass();
}

double trailer_angle(void)
{
	return trailer.trailer_angleclass();
}
//Functions for the timer
Timerclass::Timerclass(){}
Timerclass::~Timerclass(){}

	
Timerclass timer;


unsigned long Timerclass::get_timerclass(int timer_no){
return _timervar[timer_no];
}

void Timerclass::set_timerclass(int timer_no,unsigned long timerincrement)
{
  if (_timer_elapsed[timer_no])
  {
	_timervar[timer_no]=millis()+timerincrement;
	_timer_elapsed[timer_no]=false;
   // set_statei(_timerstate);
  }
  /*if (get_state()!=_timerstate)
  {
	_timervar=millis()+timerincrement;
    set_statei(_timerstate);
  }*/
}
bool Timerclass::timer_elapsedclass(int timer_no)
{
  bool elapsed=millis()>get_timerclass(timer_no);
 // if (elapsed) _timer_elapsed=true;
  _timer_elapsed[timer_no]=elapsed;
  return(elapsed); 

}


unsigned long get_timer(int timer_no){
return timer.get_timerclass(timer_no);
}

void set_timer(int timer_no,unsigned long timerincrement){
timer.set_timerclass(timer_no,timerincrement);
}

bool timer_elapsed(int timer_no){
  return timer.timer_elapsedclass(timer_no);

}
// public functions for the GPS

Gps::Gps(){
}
Gps::~Gps(){
}

void Gps::setLat(double lat)
{
  _lat = lat;
}

void Gps::setLon(double lon)
{
  _lon = lon;
}
double Gps::getLat()
{
  return _lat;
}
double Gps::getLon()
{
  return _lon;
}

void Gps::setAvailable(bool availability)
{
  _status = availability;
}

bool Gps::isAvailable()
{
 return  _status;
}


Gps GPS; // create a global instance


//###################################### wireless class methods etc,

//#####################################

Wireless::Wireless(){
  purgeWirelessBuffer();
  _myID = '1';
  _runningProgram = false;
  _debugOn = false;
  _lastMessage = "";
    
}

//######################################


Wireless::~Wireless(){
}

Wireless w;



//########################################OK

void Wireless::sendCommand(char slaveID,char messageType){

  // slaveID: an ascii character identifying the recipient of the message - x = broadcast
  // messageType: r - run a routine
  //              s - stop routine
  //              o - lon please
  //              a - lat please
  //              p - ping
  
  String message = "";
  
  message = message + slaveID + ',' + messageType; // concatenate string
  
  sendWirelessMessage(message); // use send message sending function to add error checking
  
  Serial.println("Sending command: " + message);
  
}

//############################################OK

void Wireless::sendMessage(char slaveID,String data){

  // slaveID: an ascii character identifying the recipient of the message - x = broadcast
  
  String message = "";
  
  message = message + slaveID + ',' + data; // concatenate string
  
  sendWirelessMessage(message); // use send message sending function to add error checking
  
  Serial.println("Sending message: " + message);
  
}
  
//#########################################OK

String Wireless::ping(char slaveID){

  sendCommand(slaveID,'p');
  delay(1000);
  return getWirelessMessage(); // return the response as a string
}

//#########################################OK

String Wireless::runSlaveProgram(char slaveID){

  sendCommand(slaveID,'r');
  delay(1000);
  return getWirelessMessage(); // return the response as a string
  
}

//#########################################OK

String Wireless::stopSlaveProgram(char slaveID){

  sendCommand(slaveID,'s');
  delay(1000);
  return getWirelessMessage(); // return the response as a string
  
}

//#########################################OK

double Wireless::getSlaveE(char slaveID){
  
    sendCommand(slaveID,'E');
    delay(1000);
    return getWirelessMessage().toFloat(); // return the response as a double
}

//#########################################OK

double Wireless::getSlaveN(char slaveID){

    sendCommand(slaveID,'N');
    delay(1000);
    return getWirelessMessage().toFloat(); // return the response as a double 
}

//#########################################

void Wireless::processMessage(){ // answer a command or save the message

   String message = getWirelessMessage();

   if (message[0] == 'x') {_messageAvailable = true; _lastMessage = message;} // broadcast
   if (message[0] == _myID){
		  String reply = "";
            switch (message[2]) {
                case 'p':{
                  reply = reply + _myID + " here";
                  sendWirelessMessage(reply);
				  Serial.println("Ping answered " + String(getMyID()));
                  }
                  break;
                case 'E':{ 
				  double lt = get_lat();
				  double ln = get_lon();
				  double UTMNorthing,UTMEasting;
				  deg2utm(lt,ln,&UTMNorthing, &UTMEasting);
				  
                  sendWirelessMessage(String(UTMEasting));
                  Serial.println("E sent: " + String(UTMEasting));	  
                  }
                  break;
                 case 'N':{
              	  double lt = get_lat();
				  double ln = get_lon();
				  double UTMNorthing,UTMEasting;
				  deg2utm(lt,ln,&UTMNorthing, &UTMEasting);
				  
                  sendWirelessMessage(String(UTMNorthing));
                  Serial.println("N sent: " + String(UTMNorthing));
                  }
                  break;
                 case 'r':{
                  _runningProgram = true;
				  reply = reply + _myID + " Running";
				  sendWirelessMessage(reply);
                  Serial.println("Run process");
                  }
                  break;
                 case 's':{
                  _runningProgram = false;
				  reply = reply + _myID + " Stopped";
				  sendWirelessMessage(reply);
                  Serial.println("Stop process");
                  }
                  break;
                default: 
                  _lastMessage = message.substring(2);
                  _messageAvailable = true;
                  Serial.println("Message available");
                break;
              }
    
          }//else {return "Not for me";} 
}

//#########################################

bool Wireless::messageReceived(){ // see if a message exists
  if (_messageAvailable){
    return true;
    }
    else return false;
}

//#########################################

String Wireless::getWirelessMessage(){ // get the message (strip off error check)
  
  //Serial.println("Checking for message");
  
  
  if (!Serial2.available()){return "";}
  
  String inData = "";
  long unsigned int timeout = millis();
  char received = Serial2.read();
  
  while((received != '\n')){    
    if (Serial2.available()) {
      inData = inData + received;
      received = Serial2.read();     
      }
    if ((millis() - timeout) > 1000)
        {
          return "terminator error";
        }
      }

if(_debugOn) Serial.println("Raw message received: " + inData);
      
    String lastDigit;
    if(inData.length() > 11){
    lastDigit = String(inData.length()-3);}
    else lastDigit = String(inData.length()-2);// number that should be transmitted as string


     // decode error checking
     if ((inData[inData.length()-2]) == (lastDigit[lastDigit.length()-1])){
        
        if(inData.length() > 11) {return inData.substring(0,inData.length()-3);} 
        else {return inData.substring(0,inData.length()-2);}
      }
      else {return "corruption error";}
}

//#########################################

void Wireless::sendWirelessMessage(String message){ // send a message
  
  Serial2.println(message + String(message.length())); // add error checking
  
 if(_debugOn) Serial.println("Raw message sent: " + message + String(message.length()));
  
}

//#########################################

int Wireless::purgeWirelessBuffer(){ // purge any existing messages or garbage
  
  int counter = 0;
  while (Serial2.available()){
    delay(10); // clean up any message in mid-transmission too
    char dump = Serial2.read();
    counter ++;
  }
  return counter;
}

//#########################################

void Wireless::setMyID(char myID){

  _myID = myID;
  
}

//#########################################

char Wireless::getMyID(){

  return _myID;
  
}

//#########################################

String Wireless::getMessage(){
  
  _messageAvailable = false;
  return _lastMessage;
  
}

//#########################################

bool Wireless::runProgram(){
  
  if (_runningProgram) return true; else return false;
  
}

void Wireless::showDebugInfo(bool turnOn){

if (turnOn) _debugOn = true; else _debugOn = false;

}


//Timer2 Overflow Interrupt Vector, called every 1ms
ISR(TIMER2_OVF_vect) {

  static char gpsstring[100];
  static char pcstring[100];
  static int readinggps=0,readpos=0;
  static int readingpc=0,readpcpos=0;
  char c;
  double la=0,lo=0;
  
  if (Serial1.available())
  {
    c=Serial1.read();
    //Serial.print(c);
    if (c=='$') {readinggps=1;readpos=0;}
    if (readinggps)gpsstring[readpos++]=c;
    //Serial.println(gpsstring);
    if (c=='\r') 
    {
      //Serial.println(gpsstring);
      readinggps=0;gpsstring[readpos]=0;
      if (gpsstring[3]=='R')
      {        
          la= convlat(gpsstring);
          lo= convlon(gpsstring);
	  GPS.setLat(la);
	  GPS.setLon(lo);
	  if (la > 1) GPS.setAvailable(true); // no good in southern hemisphere!
      }
    }
  }//if Serial1.available...
 
  TCNT2 = 130;           //Reset Timer to 130 out of 255
  TIFR2 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
}; 




void steer(double steer_angle)
{
  steer_angle=-steer_angle+90; //Central = 90
  if (steer_angle>steer_angle_max) steer_angle=steer_angle_max;
  if (steer_angle<steer_angle_min) steer_angle=steer_angle_min;
  steerservo.write(steer_angle);
}

void drive(float drive_speed) // input is < 1 Metre per Sec
{
  int new_num = drive_speed * 100; // x 100 to simplify and use integers
  
  if (new_num > 0) {
  
	  new_num = constrain(new_num,10,100);
	  drive_speed = map(new_num,10,100,10,45);  
  }
  
  if (new_num < 0) {
  
	  new_num = constrain(new_num,-100,-10);
	  drive_speed = map(new_num,-100,-10,-45,-10);   
  }
  
  //drive_speed=drive_speed+90; //stopped = 90]
  drive_speed = drive_speed + 90;
  driveservo.write(drive_speed);
}

void drive(float drive_speed, byte accel) // input is < 1 Metre per Sec
{
  int new_num = drive_speed * 100; // x 100 to simplify and use integers
  
  if (new_num > 0) {
  
	  new_num = constrain(new_num,10,100);
	  drive_speed = map(new_num,10,100,10,45);  
  }
  
  if (new_num < 0) {
  
	  new_num = constrain(new_num,-100,-10);
	  drive_speed = map(new_num,-100,-10,-45,-10);   
  }
  
  //drive_speed=drive_speed+90; //stopped = 90]
  drive_speed = drive_speed + 90;
  driveservo.write(drive_speed,accel);
}

void turret(int steer_angle)
{
  steer_angle=steer_angle+90; //Central = 90
  if (steer_angle>turret_max) steer_angle=turret_max;
  if (steer_angle<turret_min) steer_angle=turret_min;
  turretservo.write(steer_angle);
}

void turret(int steer_angle,byte speed)
{
  steer_angle=steer_angle+90; //Central = 90
  if (steer_angle>turret_max) steer_angle=turret_max;
  if (steer_angle<turret_min) steer_angle=turret_min;
  turretservo.write(steer_angle,speed);
}

void turret(int steer_angle,byte speed,bool block) // block pauses program until position reached
{
  steer_angle=steer_angle+90; //Central = 90
  if (steer_angle>turret_max) steer_angle=turret_max;
  if (steer_angle<turret_min) steer_angle=turret_min;
  turretservo.write(steer_angle,speed,block);
}



long read_ultrasonic()
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds on the 'trig' pin.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigpin, LOW);

  // The echo pin is used to read the signal from the sensor a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  
  duration = pulseIn(echopin, HIGH);

  // convert the time into a distance cm
  
	return duration / 29 / 2;
 
}

void clear_LCD(){
  Serial3.write(0xFE);Serial3.write(0x01); //Clear LCD
}
void nextline_LCD(){
  Serial3.write(0xFE);Serial3.write(192); //Next LCD Line
}

double read_compass(void)
{
   byte highByte, lowByte, fine;              // highByte and lowByte store high and low bytes of the bearing and fine stores decimal place of bearing
   float bearing;                               // Stores full bearing
   
   Wire.beginTransmission(I2C_Address);           //starts communication with CMPS10
   Wire.write(2);                             //Sends the register we wish to start reading from
   Wire.endTransmission();

   Wire.requestFrom(I2C_Address, 2);              // Request 4 bytes from CMPS10
   while(Wire.available() < 2);               // Wait for bytes to become available
   highByte = Wire.read();
   lowByte = Wire.read();
 
   bearing = ((highByte<<8)+lowByte)/10;      // Calculate full bearing
   return double(bearing); 
  
}


double closest_bearing_difference(double current_bearing,double goal_bearing)
{
    double cl_bearing;
    cl_bearing=goal_bearing-current_bearing;
    if (cl_bearing>180) cl_bearing=(cl_bearing-360);
    if (cl_bearing<-180) cl_bearing=(cl_bearing+360);
    return cl_bearing;
}
double resolve_bearing(double bearing)
{
    if (bearing>360) bearing=bearing-360*int(bearing/360);
    if (bearing<0) bearing=bearing+360*(int(-bearing/360)+1);
    return bearing;
}

double bearing_to_point(double current_northing,double current_easting,double goal_northing,double goal_easting)
{
  double bear_pt;
  bear_pt=double(atan2(goal_easting-current_easting,goal_northing-current_northing)/M_PI*180);
  bear_pt=resolve_bearing(bear_pt);
  return bear_pt;
}

double distance_to_point(double current_northing,double current_easting,double goal_northing,double goal_easting)
{
	double dist;
	dist=double(sqrt((current_northing-goal_northing)*(current_northing-goal_northing)+(current_easting-goal_easting)*(current_easting-goal_easting)));
	return dist;
}
                      
 double convlat(char s[])
  {
    int readcommapos=0,commafound=0;
    do {
     // readcommapos++;
      if (s[++readcommapos]==',') commafound++;
    }while (commafound<3);
   
  char dd[4];
  char mm[10];
  double latd,latm,lattot;
  dd[0]=s[1+readcommapos];
  dd[1]=s[2+readcommapos];
  dd[2]=0;
  mm[0]=s[3+readcommapos];
  mm[1]=s[4+readcommapos];
  mm[2]=s[5+readcommapos];
  mm[3]=s[6+readcommapos];
  mm[4]=s[7+readcommapos];
  mm[5]=s[8+readcommapos];
  mm[6]=s[9+readcommapos];
  mm[7]=0;  
   latd=strtod(dd,NULL);
  latm=strtod(mm,NULL);

  lattot=latd+(latm/60);
 
  return lattot;
  }
  
double convlon(char s[])
{
  char mm[10],dd[4];
  double lond,lonm,lontot; 
  int readcommapos=0,commafound=0;
    do {
  
      if (s[++readcommapos]==',') commafound++;
    }while (commafound<5);
  
  dd[0]=s[1+readcommapos];
  dd[1]=s[2+readcommapos];
  dd[2]=s[3+readcommapos];
  dd[3]=0;
  
  lond=strtod(dd,NULL);
 
  mm[0]=s[4+readcommapos];
  mm[1]=s[5+readcommapos];
  mm[2]=s[6+readcommapos];
  mm[3]=s[7+readcommapos];
  mm[4]=s[8+readcommapos];
  mm[5]=s[9+readcommapos];
  mm[6]=s[10+readcommapos];
 
 lonm=strtod(mm,NULL);
 lontot=-(lond+(lonm/60));
  
  return lontot;
}  


//Use leds eg  digitalWrite(led1, HIGH);
void led_out(int led,bool value)
{
	if (led==1) digitalWrite(led1,!value);
	if (led==2) digitalWrite(led2,!value);
	if (led==3) digitalWrite(led3,!value);
}
int read_button(int button)
{
	int buttonno=1;
	if (button==3) buttonno=button1;
	if (button==2) buttonno=button2;
	if (button==1) buttonno=button3;
	return !digitalRead(buttonno);
}

  int getPitch(){
    
    int x; // for accelReader
    int y;
    int z;
      accelReader.readAccel(&x,&y,&z);
      x = map(x,0,255,0,90);
      return x;
  }
  
    int getRoll(){
    
    int x; // for accelReader
    int y;
    int z;
      accelReader.readAccel(&x,&y,&z);
      y = map(y,0,255,0,90);
      return y;  
  }
  
  void getEulerAngles(float* angles) {
  
  sixDOF.getEuler(angles);
  
  }

  void display(String topLine){
  
  lastMessage1 = "";lastMessage2 = "";

  if (topLine != lastMessage){

  clear_LCD();
  Serial3.print(topLine.substring(0,16));

	lastMessage = topLine;  
  }
}



void display(String topLine, String bottomLine ){

lastMessage ="";

 if ((topLine != lastMessage1)||(bottomLine != lastMessage2)){
 
  clear_LCD();
  Serial3.print(topLine.substring(0,16));
  nextline_LCD();
  Serial3.print(bottomLine.substring(0,16));
  
  lastMessage1 = topLine;
  lastMessage2 = bottomLine;
  }
}

// access the GPS
bool gps_avail(){
return GPS.isAvailable();
}
double get_lat(){
return GPS.getLat();
}
double get_lon(){
return GPS.getLon();
}

void serialEvent2() { // triggers processing of wireless by the wireless object

w.processMessage();

}

bool scan(int stand_out,int* distr, int* bearr) //Returns 999,999 if not found
{
  int n;
  int found_mode=0; //state=0 not found an object,1=object found
  bool found_anything=0;
  int start_obj,end_obj;
  int dist,prevdist;
  int scan_array_pos=0;
  int closest_obj=999,closest_obj_angle=999;
  float closest_obj_angle_f=999;
  
   found_anything=0;
   found_mode=0;
   closest_obj=999;
   turret(-80);
   Serial.print(":");
   delay(500);
   for (n=-80;n<80;n+=5)
   {
    turret(n);
    dist=read_ultrasonic();
    Serial.print(dist);
    Serial.print(",");
    delay(80);
    if (n>-80) //don't do at the start of the scan
    {
      if (((prevdist-dist)>stand_out) && (found_mode==0))
      {
        if (dist+10<closest_obj)
        {
        found_mode=1;
        start_obj=n;
        }
      }
    }
    if ((found_mode==1) && ((dist-prevdist)>stand_out))
    {
      end_obj=n;
      found_mode=0;
      found_anything=1;
      closest_obj_angle_f=start_obj+((end_obj-start_obj)/2)-10;
      closest_obj_angle=(int)closest_obj_angle_f;
    }
    if ((found_mode==1) && (dist<closest_obj)) closest_obj=dist;
    
  // if (dist<closest_obj) {closest_obj=dist;closest_obj_angle=n;}
      prevdist=dist;
   }
   *distr=closest_obj_angle;
   *bearr=closest_obj;
   return found_anything;
}

void initialise() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
   led_out(1,LOW);
   led_out(2,LOW);
   led_out(3,LOW);
  

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  
   accelReader.init(0x53);

  steerservo.attach(steerservopin);
  steerservo.write(90);
  driveservo.attach(driveservopin);
  //set_trailer_centre();
  drive(0);
  
  delay(1100); //Hold in Stopped position for 1.1 secs, required for the motor servo to work
  turretservo.attach(turretservopin);
  turret(0);
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  //LCD
  Serial3.begin(9600);
  //Compass
   Wire.begin();
  //GPS
	Serial1.begin(4800);  
	Serial.begin(9600);
	Serial2.begin(1200); // wireless
	
	 sixDOF.init(); //begin the IMU
     Serial3.print("");
	 
	  //Setup Timer2 to fire every 1ms
  TCCR2B = 0x00;        //Disbale Timer2 while we set it up
  TCNT2  = 130;         //Reset Timer Count to 130 out of 255
  TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
  TCCR2A = 0x00;        //Timer2 Control Reg A: Normal port operation, Wave Gen Mode normal
  TCCR2B = 0x05;        //Timer2 Control Reg B: Timer Prescaler set to 128

}


