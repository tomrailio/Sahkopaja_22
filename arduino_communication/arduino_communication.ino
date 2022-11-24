/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
#include <FastLED.h>
#include <Servo.h>

// LED configs
#define NUM_LEDS 6
#define DATA_PIN 3
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS 100

String state = "";
unsigned long previousTime = millis();
const int blinkInterval = 500;
unsigned long blinkStart = millis();
const int blinkTime = 5000;
CRGB leds[NUM_LEDS];

// Servo configs
int servo_position = 0;
int serv_down = 0;
int serv_up = 120;
Servo theservo;

// OLED Screen configs
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

// the setup function runs once when you press reset or power the board
void setup() {
  // LED setup
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setBrightness( BRIGHTNESS);
  FastLED.addLeds<TM1829, DATA_PIN, RGB>(leds, NUM_LEDS);
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // SERVO setup
  theservo.attach(9);
  theservo.write(servo_position);

  // LCD setup
  // SHOULD START WITH POWER OFF?
  // PROBLEM: MAKE CHANGES TO CIRCUIT SO A COMMAND CAN POWER ON THE LCD DISPLAY
  // FOR NOW: ASSUME DISPLAY IS ALWAYS ON
  display.resetOLED();  // consider a startup image

}


// LED LIGHT ROUTINES

bool rightIsOn() {
  return leds[4] == CRGB(0, 255, 0);
}

bool leftIsOn() {
  return leds[0] == CRGB(0, 255, 0);
}
void turnOffLights(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(0,0,0);
  }
}

void prepareLights(String s) {
  state = s;
  turnOffLights();
  blinkStart = millis();
}

// LCD SCREEN ROUTINES

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
    data = data + 'C';  // assumes centigrade temperature
  }
  if (cmd == "TIME") {  // time can get printed as raw data
    // heuristic center
    // display.setCursor(36, 12);  // set in reset function; either data format is shown centered
  }
  
  display.println(data);
  display.display();
  
  delay(t);

}

int k = 0;

// the loop function runs over and over again forever
void loop() {
  if(Serial.available() > 0){
    String data = Serial.readStringUntil('\n');
    String command = data.substring(0, data.indexOf(" "));
    Serial.print("You sent me: ");
    if(command == "OPEN"){  // OPEN THE VISOR
      //digitalWrite(LED_BUILTIN, HIGH);
      //leds[0] = CRGB(255,0,0);

      for (k = servo_position; k > serv_down; k--) {
        theservo.write(k);
      }
      servo_position = k;
    } else if(command == "CLOSE"){  // CLOSE THE VISOR
      //digitalWrite(LED_BUILTIN, LOW);
      //leds[0] = CRGB(0,0,0);

      for (k = servo_position; k < serv_up; k++) {
        theservo.write(k);
      }
      servo_position = k;
    } else if(command == "RIGHT"){
      prepareLights("RIGHT");
      for (int i = 4; i < NUM_LEDS; i++){
        leds[i] = CRGB(0, 255, 0);
      }
      //previousTime = millis();
    } else if(command == "LEFT"){
      prepareLights("LEFT");
      for(int i = 0; i < NUM_LEDS / 2; i++) {
        leds[i] = CRGB(0, 255, 0);
      }
    } else if (command == "WEATHER" || command == "TIME") {
      display.resetOLED();
      display.writeOLED(data);  // NOTE: input is DATA, and not COMMAND
  }


  if(millis() - blinkStart < blinkTime){  // WARNING: may cause LCD screen bugs?
    if (state == "RIGHT"){
      if(millis() - previousTime >= blinkInterval){
        //Serial.println("BLINKING");
        CRGB color;
        previousTime = millis();
        if(rightIsOn()){
          color = CRGB(0,0,0);
        } else {
          color = CRGB(0, 255, 0);
        }
        for(int i = NUM_LEDS / 2; i < NUM_LEDS; i++){
          leds[i] = color;
        }
        
      }
    }
  
    if(state == "LEFT"){
      if(millis() - previousTime >= blinkInterval){
        CRGB color;
        previousTime = millis();
        if(leftIsOn()) {
          color = CRGB(0,0,0);
        } else {
          color = CRGB(0, 255, 0);
        }
        for(int i = 0; i < NUM_LEDS / 2; i++){
          leds[i] = color;
        }
      }
    }
  } else {
    turnOffLights();
    state = "";
  }
    FastLED.show();
    //delay(100);
    //digitalWrite(LED_BUILTIN, LOW);
    // delay(500);
  
}
