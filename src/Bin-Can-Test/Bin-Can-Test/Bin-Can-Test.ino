

// CAN Receive Example
//POD - SLOW!!! VERSION
//
//    if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)
//

//
//        LOWER   7B
//        UPPER   91
//

void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  
  unsigned char upperByte = 0x91;
  unsigned char lowerByte = 0x7B;
  signed short int result;
  result = (upperByte << 8) | lowerByte;
  Serial.println(  result/1024.0 );
}

void loop() {
  // put your main code here, to run repeatedly:

}
