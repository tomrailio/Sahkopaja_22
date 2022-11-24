/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

 /*
  * This is based on example code provided by Adafruit but adapted for a custom purpose.
  * 1. Program must obtain text from Serial
  * 2. Program must display text on Screen for a set time interval
  * 3. Program must have the capability to change display text based on Serial interrupts
  * 
  */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

static const unsigned char PROGMEM fett [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1C, 0x0C, 0x04, 0x04,
0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE1, 0xC0, 0xE0, 0xE2, 0xE0, 0xC4, 0x80, 0x08, 0x10,
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xE1, 0x00, 0x10, 0x03, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0x03, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00,
0x00, 0x00, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0xFF, 0x3F, 0x00, 0x02, 0x22, 0x20,
0x60, 0x60, 0x42, 0x40, 0xC0, 0xC0, 0xC0, 0xE0, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x60, 0x60, 0x60, 0x22, 0x20, 0x20,
0x20, 0x12, 0x10, 0x10, 0x01, 0x02, 0xF8, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x7F, 0x3F, 0x01, 0x00, 0x00, 0x00, 0x04,
0x08, 0x00, 0x10, 0x20, 0x48, 0x08, 0x00, 0x00, 0x00, 0x01, 0x11, 0x21, 0x41, 0xF3, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x04, 0x24, 0x10,
0x00, 0x08, 0x04, 0x04, 0x00, 0x02, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00,
0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x06, 0x30, 0x80, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x84, 0x4C, 0x12, 0x08, 0x00, 0x02, 0x00,
0x02, 0x04, 0x00, 0x20, 0x00, 0x00, 0x00, 0x7F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0D, 0x10, 0x28, 0x00, 0x00, 0x00, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0x00, 0x00, 0x21, 0x18, 0x00, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x01, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x20, 0x21, 0x40, 0x00, 0x00, 0x7F, 0x7F, 0x7F,
0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x40, 0x40, 0x00, 0x37, 0x00, 0x00, 0x08, 0x00, 0x00, 0x02, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM fett2 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x40, 0x38, 0x04,
0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x30, 0x38, 0x1C, 0x1C, 0x1E, 0x3C, 0x3E, 0x3E, 0x3E, 0x3C, 0x38, 0x00, 0x01,
0x02, 0x04, 0x18, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x0C, 0xFE, 0xFE, 0x02, 0x21, 0x20, 0x00,
0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
0x00, 0x20, 0x00, 0x00, 0x1F, 0x20, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0xFF, 0x13, 0x00, 0x02, 0x02, 0x46,
0x86, 0x06, 0x06, 0x0E, 0x8E, 0x8E, 0x0E, 0x0E, 0x1E, 0x1E, 0x1E, 0x1E, 0x3E, 0x3E, 0xFE, 0xFE,
0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x3E, 0x3E, 0x3E, 0x1E, 0x0E, 0x0E, 0x06, 0x46, 0x46, 0x02,
0x03, 0x83, 0x41, 0x41, 0x00, 0x20, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0x07, 0x03, 0x00, 0x00, 0x00, 0x80, 0x00,
0x00, 0x00, 0x01, 0x42, 0x04, 0x10, 0x60, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x40, 0xC0, 0x20, 0x80, 0x00, 0x22, 0x01,
0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM fett2a [] = {
0x00, 0x10, 0x40, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x80, 0x00, 0x01, 0x70, 0x80, 0x00,
0x00, 0x11, 0x00, 0x00, 0x07, 0xF8, 0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x0F, 0xFC, 0x20, 0x00,
0x00, 0x12, 0x00, 0x00, 0x1F, 0xFC, 0x20, 0x00, 0x00, 0x02, 0x00, 0x00, 0x18, 0xFC, 0x10, 0x00,
0x00, 0x04, 0x00, 0x60, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x10, 0x00,
0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00,
0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00,
0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0xB6, 0x20, 0x00, 0x00, 0x04, 0x44, 0x00,
0x00, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
0x00, 0xB0, 0x00, 0x00, 0x00, 0x00, 0xF2, 0x00, 0x00, 0xB7, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0x00,
0x00, 0xA1, 0xFF, 0xFF, 0xFF, 0xFE, 0x03, 0x00, 0x00, 0xA0, 0x1F, 0xFF, 0xFF, 0xF0, 0x03, 0x00,
0x00, 0xF0, 0x00, 0xFF, 0xFF, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x00, 0x0F, 0xFF, 0x80, 0x07, 0x00,
0x00, 0xE1, 0x00, 0x03, 0xFC, 0x06, 0x33, 0x00, 0x00, 0xE0, 0x8C, 0x03, 0xFC, 0x00, 0x43, 0x00,
0x00, 0xE0, 0x20, 0x27, 0xFC, 0x01, 0x03, 0x00, 0x00, 0xE0, 0x10, 0x17, 0xFC, 0x02, 0x01, 0x00,
0x00, 0x40, 0x08, 0x0F, 0xFC, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x01, 0x00,
0x00, 0x80, 0x04, 0x07, 0xFC, 0x00, 0x01, 0x00, 0x00, 0x80, 0x02, 0x07, 0xF8, 0x12, 0x81, 0x00,
0x00, 0x80, 0x12, 0x07, 0xF8, 0x60, 0x41, 0x00, 0x00, 0x82, 0x00, 0x07, 0xF8, 0x28, 0x01, 0x00
};

static const unsigned char PROGMEM fett3 [] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80,
0x80, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  // display.clearDisplay();

  // Draw a single pixel in white
  //display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  //display.display();
  //delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  // STARTUP CODE
  // Serial.readStringUntil('\n');
  display.clearDisplay();
  display.drawBitmap(32,0,fett2a,64,32,1);  // not a good picture but works
  display.display();
  
  delay(3000);
  
  String txt = "TIME 12:06";

  resetOLED();
  writeOLED(txt);
  // testscrolltext();    // Draw scrolling text
}

void loop() {  // no loop currently, text gets left on screen

  int response = readwriteOLED('\n');
  delay(10000);  // maybe too bad resolution
  
}

int readwriteOLED(char terminator) {
  
  if(Serial.available() > 0) {
    String data = Serial.readStringUntil(terminator);  // should check command and display based on that
    resetOLED();
    writeOLED(data);
    return 1;
  }
  else {
    return 0;
  }
  
}


// tentative WIP function not sure if we will use this
int stateControlOLED(int state, String txt = "dummy") {
  // expand this function later if needed
  // possibly stupid solution
  // controls whether the display shows something or updates something
  // state 0 -> display text
  // state 1 -> update from serial, after update is done swap back to 0

  int response;
  int nextstate;
  //int nextstate = checkResponse(state);

  if (state == 0 && nextstate == 0) {
    return nextstate;
  }
  else if (state == 0 && nextstate == 1) {
    resetOLED();
    //updateOLED();
  }
  else if (state == 1 && nextstate == 0) {
    //
//    writeOLED();
  }

  return response;
}

void resetOLED() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(36, 12);
  display.dim(true);
  
}


void writeOLED(String s) {
  // should display only, or display and clear screen?
  // the returning string s har form "CMD DATA"
  // screen should display DATA only 
  // t = duration?
  int t = 1000;

  int spcidx = s.indexOf(' ');
  int slen = s.length();
  String cmd = s.substring(0, spcidx);
  String data = s.substring(spcidx+1,slen);

  if (cmd == "WEATHER") {
    data = data + 'C';
  }
  if (cmd == "TIME") {
    // heuristic center
    // display.setCursor(36, 12);
  }
  
  display.println(data);
  display.display();
  
  delay(t);

}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 8);
  display.println(F("Bullshit"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}
