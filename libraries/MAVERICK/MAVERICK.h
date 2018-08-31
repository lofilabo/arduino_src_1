/*
Maverick robot library
For the Summer School Robotics
GPS Robotics
Created by Samuel Wane, Harper Adams University, August 2015
Library folder adapted by Matt Butler March 2016
Updated for wireless summer 2016
*/


#ifndef MAVERICK_h
#define MAVERICK_h

#include "Arduino.h"

//Includes within header files can be problematic with Arduino IDE.
//dependent sub-libraries (and utility folders) must be inside the Maverick folder
#include <Wire.h>
#include <VarSpeedServo.h>
#include "deg2utm.h"

#include "FreeSixIMU.h"


#include <math.h>
#include <stdlib.h>


//classes
class Trailerclass
{
	public:
		Trailerclass();
		~Trailerclass();
		double trailer_angleclass(void);
		void set_trailer_centreclass(void);
	private:
		double _trailer_offset=-1;
};

class Timerclass
{
	public:
		Timerclass();
		~Timerclass();
		unsigned long get_timerclass(int timer_no);
		void set_timerclass(int timer_no,unsigned long timerincrement);
		bool timer_elapsedclass(int timer_no);
	private:
		unsigned long _timervar[10]={0,0,0,0,0,0,0,0,0,0};
		bool _timer_elapsed[10]={true,true,true,true,true,true,true,true,true,true};
};

class Gps
{
  public:
    Gps();
    ~Gps();
    void setLat(double lat);
    void setLon(double lon);
    double getLat();
    double getLon();
    bool isAvailable();
    void setAvailable(bool availability);
  private:
    double _lat = 0;
    double _lon = 0;
    bool _status = false;
};


class Wireless
{
  public:
    Wireless(); // wireless object constructor
    ~Wireless(); // wireless object destructor
    void sendMessage(char slaveID, String data); // slave id 'x' is broadcast
    String ping(char slaveID);                  // expect response like " 4 here"
    double getSlaveE(char slaveID);           // use .toFloat()
    double getSlaveN(char slaveID);
    void processMessage();             // this is called automatically every time serial2 gets a byte
    bool messageReceived();              // used to find out if a message is available
    String getMessage();   				 //then collect it
	void setMyID(char myID);             // default is '1'. User should set.
    char getMyID();                      // to check current ID set
    int purgeWirelessBuffer();           // clear serial 2 buffer. Performed automatically at start
    bool runProgram();  // should my program run?
    String runSlaveProgram(char slaveID); // start or stop program on slave
    String stopSlaveProgram(char slaveID);
	void showDebugInfo(bool turnOn); // see all communication
  private:
    bool _messageAvailable; // indicates a message is available
    void sendWirelessMessage(String message); // used internally
    String getWirelessMessage();              // used internally
    void sendCommand(char slaveID,char messageType); // used internally
    char _myID;          // use public get and set methods
    String _lastMessage; // the last message received
    bool _runningProgram;    // used internally
	bool _debugOn;		 // used internally to store debug status
};

extern Wireless w; // wireless object globally available


//Forward declarations
double trailer_angle();
void set_trailer_centre();
unsigned long get_timer(int timer_no);
void set_timer(int timer_no,unsigned long timerincrement);
bool timer_elapsed(int timer_no);
bool gps_avail();
double get_lat();
double get_lon();

void initialise();
void steer(double steer_angle);

void drive(float drive_speed);
void drive(float drive_speed,byte accel);

void turret(int angle);
void turret(int steer_angle,byte speed);
void turret(int steer_angle,byte speed,bool block);

long read_ultrasonic();
void clear_LCD();
void nextline_LCD();
double read_compass(void);
double closest_bearing_difference(double current_bearing,double goal_bearing);
double bearing_to_point(double current_northing,double current_easting,double goal_northing,double goal_easting);
double distance_to_point(double current_northing,double current_easting,double goal_northing,double goal_easting);
//int read_gps(double* lat,double* lon);
double convlat(char s[]);
double convlon(char s[]);
int read_button(int button);
void led_out(int led,bool value);


int getRoll();
int getPitch();

void getEulerAngles(float* angles);

void display(String topLine);
void display(String topLine, String bottomLine ); // override
bool scan(int stand_out,int* distr, int* bearr); //Returns 999,999 if not found
#endif

