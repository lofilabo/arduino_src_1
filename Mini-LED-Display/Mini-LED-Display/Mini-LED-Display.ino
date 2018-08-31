#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int i=0;
int j=0;

void setup() {
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  // display a pixel in each corner of the screen
  display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 63, WHITE);
  display.drawPixel(127, 63, WHITE);

  // display a line of text
  /*
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(27,30);
  display.print("Hello, world!");
  */
  // update display with all of the above graphics
  display.display();
}

void loop() {

  //long_string();
  //track_lr();
  //basic_circle();
  basic_triangle();
  //basic_square();

}

void basic_circle(){

  display.clearDisplay();
  display.drawCircle(60, 30, 16, WHITE);
  display.drawCircle(60, 30, 16, WHITE);
  //display.fillCircle(60, 30, 16, WHITE);
  display.display();
  
}

void basic_square(){

  display.clearDisplay();
  display.drawRect(60, 30, 32, 32, WHITE);
  //display.fillCircle(60, 30, 16, RED);
  display.display();
  
}

void basic_triangle(){

  display.clearDisplay();
  display.drawTriangle( 40, 60, 60, 20, 80, 60, WHITE);
  //display.fillTriangle(60, 30, 16, WHITE);
  display.display();
  
}

void long_string(){

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(1,2);
  display.print("abcdefghijklmnopqrstuvwxyz0123456789\|/#%");
  display.display();
  delay(0);

  
}

void track_lr(){

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(i,j);
  display.print("A");
  display.display();
  delay(0);
  i++;
  if(i>116){
    i=1;
    j=j+16;
  }
  if(j>60){
    j=1;  
  }
  
}

