
#include<SoftwareSerial.h>// soft serial port header file

SoftwareSerial SerialS(2,3); // define the soft serial port as Serial1, pin2 as RX, and pin3 as TX

/*For Arduino board with multiple serial ports such as DUE board, comment out the above two codes, and directly use Serial1 port*/

int dist;// LiDAR actually measured distance value
int strength;// LiDAR signal strength
int check;// check numerical value storage
int i;
int uart[9];// store data measured by LiDAR
const int HEADER=0x59;// data package frame header
void setup(){
  Serial.begin(9600);//set the Baud rate of Arduino and computer serial port
  SerialS.begin(115200);//set the Baud rate of LiDAR and Arduino serial port
}

void loop(){

    if (SerialS.available()){//check whether the serial port has data input
        if(SerialS.read()==HEADER){// determine data package frame header 0x59
          uart[0]=HEADER;
              if(SerialS.read()==HEADER){//determine data package frame header 0x59
                uart[1]=HEADER;
                for(i=2;i<9;i++){// store data to array
                  uart[i]=SerialS.read();
                }
              check=uart[0]+uart[1]+uart[2]+uart[3]+uart[4]+uart[5]+uart[6]+uart[7];
        
                if(uart[8]==(check&0xff)){// check the received data as per protocols
                  dist=uart[2]+uart[3]*256;// calculate distance value
                  strength=uart[4]+uart[5]*256;// calculate signal strength value
                  Serial.print("dist = ");
                  Serial.print(dist);// output LiDAR tests distance value
                  Serial.print('\t');
                  Serial.print("strength = ");
                  Serial.print(strength);// output signal strength value
                  Serial.print('\n');
                 }
             }
      }
   }
}
/*
Step 3: Data View 
    Upload the program to the Arduino board and open the serial monitor to check the distance value 
detected by LiDAR in real time and corresponding signal strength as in Figure 3; 
    Besides, you can also check the data curves on the serial curve plotter, but for that you need to 
modify the code related to serial printing (Serial.print):
*/
/*
//         Serial.print("dist = ");

            Serial.print(dist);// output LiDAR tests distance value

            Serial.print(' ');

//         Serial.print("strength = ");

            Serial.print(strength);// output signal strength value

            Serial.print('\n');
*/
