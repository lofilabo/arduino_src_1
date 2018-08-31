    #include <SPI.h>
 #include <mcp_can.h>

    int canId = 0x000;

    unsigned char len = 0;
    unsigned char buf[8];
    char str[20];


    void setup()
    {
        Serial.begin(38400);

    START_INIT:

        if(CAN_OK == CAN.begin(CAN_250KBPS))
        {
            Serial.println("Initialized successfully");
        }
        else
        {
            Serial.println("Initializing is failed");
            Serial.println("Reloading...");
            delay(100);
            goto START_INIT;
        }
    }


    void loop()
    {
        if(CAN_MSGAVAIL == CAN.checkReceive()) 
        {
            CAN.readMsgBuf(&len, buf);
            canId = CAN.getCanId();
            Serial.print("<");Serial.print(canId);Serial.print(",");
            for(int i = 0; i<len; i++)
            {
                Serial.print(buf[i]);Serial.print(",");
            }
            Serial.print(">");
            Serial.println();
        }
    }
