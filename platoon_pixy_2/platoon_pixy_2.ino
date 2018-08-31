//code taken from the pixy 'hello world' example

#include <SPI.h>  
#include <Pixy.h>
#include <MAVERICK.h>


//------------------------------------------//

#define signatureOfInterest 1 //3 //change this value to match the signature of the objects which are interesting to the end user
#define intertapeDistance 180.00 //this is an arbitrary scaler which will set the point where the robot decides to stop when the 
                                 //two objects it is tracking reach this distance - x axis max is 320, y axis max is 200
#define xPlane 1  //this flag shows which orientation the camera is in- if xPlane is 1, 
                  //use the x-axis to control robot turning, if xPlane = 0, use y-axis
#define speedScalar 0.3 //0.7 //this limits the ultimate speed that the robot tries to attain when in autonomous mode

//------------------------------------------//

// This is the main Pixy object 
Pixy pixy;
Block dataBuffer1[1]; //a buffer to save block data in, 10 historic values
Block dataBuffer2[1]; //we'll only track two objects for now
//buffers to store motion data in- the format should look like 
//{header, joystick x-axis byte 1, joystick x-axis byte 0, joystick y-axis byte 1, joystick y-axis byte 0, joystick button byte 0, sum}
//in order to use the joystick values, compute the following: 
//joystick-axis = {[(joystick-axis byte 1)*256 + joystick-axis byte 0]/32767 - 1.000013}
uint16_t fwdSpeed = 0;
uint16_t sSpeed = 0;
uint8_t fwdSpeedByte0 = 0;
uint8_t fwdSpeedByte1 = 0;
uint8_t sSpeedByte0 = 0;
uint8_t sSpeedByte1 = 0;
uint8_t button = 0;
long timerStart = 0;
long stopTimer = 0;
long searchTimer = 0;
int x,y;
int error=0;
float Integral=0;
float derivative=0;
float error_previous=0;
int error_s=0;
float Integral_s=0;
float derivative_s=0;
float error_previous_s=0;
int area=0;
int newarea=0;
 uint16_t xHeading, yHeading;
 int width;
 int height;
 int maxArea = 0;
 int minArea = 0;
 int SIZE;
  uint16_t blocks;//a variable to store blocks (struct) in
  //    uint16_t xHeading, yHeading;
  
void setup()
{
  
  Serial.begin(9600);//get ready to send and receive serial data on digital pins 0 and 1
  Serial.println("Starting Program Now...");
  pixy.init();//turn on the pixy camera
  //timerStart = millis();
  //searchTimer = timerStart;
  initialise();
  
}

void loop()
{ 
  
        static int i = 0;//a pixy camera frame counter, static makes the value be saved between loops
        // grab blocks!
        blocks = pixy.getBlocks(1); //(2);//can input uint16_t that specifies the number of blocks to get. 
        //These are returned in the order of signature (all the '1's first, etc.) and then size (within a signature, largest first).
        
        // Blocks are only captured once per camera frame
        //so we only update if we've read in new data
        if (blocks){
              //Serial.println("blocks...");
              i++;   
              // update the block data buffers every xx frames
              //by default, the pixy runs at 50 fps, 640x400
              if (i%10==0){
                    //loop through all blocks which have been received
                    //check the signature of each one vs the desired signatureOfInterest
                    //check the position, size and trajectory of each object buffer in order to determine which buffer to save data in
                    int distanceBetweenObjects;
                    
                    for (int j=0; j<blocks; j++){ //FOR int j=0; j<blocks; j++
                            for_each_j( j );
                    }//FOR int j=0; j<blocks; j++
              } //End i%30==0
        } //End if (blocks)
  
}

void for_each_j( int j ){
  
        Serial.print("Sig: ");
        Serial.print(pixy.blocks[j].signature);
        Serial.print("\t\t");
        if(pixy.blocks[j].signature == signatureOfInterest){ //If...signature-of-interest
              //largest detected object
              if(j == 0){
                      on_j_is_zero(j);
              } //End If...j==0
        } //END IF...signature-of-interest  
}

void on_j_is_zero( int j ){

          //rotateBlockBuffer(dataBuffer1);//update the data buffer to accept new data
          dataBuffer1[0] = pixy.blocks[j];//put the new data in the buffer
          xHeading = dataBuffer1[0].x;
          x=xHeading;
          yHeading = dataBuffer1[0].y;
          y=yHeading;
          width = dataBuffer1[0].width;
          height = dataBuffer1[0].height;
          SIZE=width*height;
          
          debug();
          float p;
          float kp=0.1875;
          float ki=0.3*kp;
          float kd=0.3*kp;
          error=xHeading-160;
          Integral=(2/3)*Integral+error;
          derivative=error-error_previous;
          p=kp*(error)+ki*Integral+kd*(derivative);
          
          if (p>30){
            p=30;
          }
          
          if (p<0){
            p=1.8*p;
          }
          
          steer(p);
          
          error_previous=error;
          
          forward();
}

void debug(){
            Serial.print("x:");
            Serial.print(xHeading);
            Serial.print("\t");
            Serial.print("y:");
            Serial.print(yHeading);
            Serial.print("\t");
            Serial.print("size:");
            Serial.print(SIZE);
            Serial.print("\t\t");  
}

void forward()
{
      float p_s;
      float kp_s=0.00033;
      float ki_s=0.4*kp_s;
      float kd_s=0.15*kp_s;
      error_s=SIZE-1200; 
      Integral_s=(2/3)*Integral_s+error_s;
      derivative_s=error_s-error_previous_s; 
                
      p_s=kp_s*(error_s)+ki_s*Integral+kd_s*(derivative);
      if (p_s<-0.3){
          p_s=-0.3;
      }
       
      if (p_s>0){
          p_s=0;
      }
      
      drive(p_s);
      error_previous_s=error_s;
      Serial.print("Previous Error:");
      Serial.println(p_s);
  }


