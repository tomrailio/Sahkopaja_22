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

// UNIT TESTING
#define __ASSERT_USE_STDERR

#include <assert.h>

// LED configs
#define NUM_LEDS 17
#define DATA_PIN 3
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS 50

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

Servo screenServo;
bool screenUp = true;
int screenPos = 100;
int screenUpPos = 100;
int screenDownPos = 8;

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


// Accelerometer configs
int x, y, z, z2, y2, z3, y3 ; 
const unsigned long eventInterval = 100;
unsigned long prevTimeAcc = 0; // previously conflicted with LED code
unsigned long valiaika = 0;
bool mittaus = true;
bool mittaus2 = true;
bool mittaus3 = true;
unsigned long valiaika2 = 0;


// Speaker configs
#define NOTE 277
#define melodyPin 5  // NOTE: CHANGED 3->5 BECAUSE LED DATA PIN IS ALREADY PIN 3

unsigned int speakerState = 0;
unsigned long sojournTime = 0;


// BOBA FETT

static const unsigned char PROGMEM fett [] = {
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



// the setup function runs once when you press reset or power the board
void setup() {
  // LCD setup
  // SHOULD START WITH POWER OFF?
  // PROBLEM: MAKE CHANGES TO CIRCUIT SO A COMMAND CAN POWER ON THE LCD DISPLAY
  // FOR NOW: ASSUME DISPLAY IS ALWAYS ON
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.drawBitmap(32,0,fett,64,32,1);  // not a good picture but works
  display.display();
  
  delay(3000);
  
  // LED setup
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness( BRIGHTNESS);
  FastLED.addLeds<TM1829, DATA_PIN, RGB>(leds, NUM_LEDS);
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  //Serial.println("This is a setup!");

  // SERVO setup
  theservo.attach(9);
  theservo.write(servo_position);

  screenServo.attach(10);
  screenServo.write(screenPos);


  // accelerometer & speaker setup
  pinMode(melodyPin, OUTPUT);//buzzer
  //pinMode(13, OUTPUT);//led indicator when singing a note

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





int checkStatus(int z, int y, int debug = 0) {
  // JL: function to verify whether helmet has moved since an impulse
  // returns 1 if helmet has moved by more than tolerance (int)
  // returns 0 otherwise

  if (debug == 1) {
    return 0; // testing this
  }

  int response = 1;

  int newz = analogRead(2);
  int newy = analogRead(1);

  int tolerance = 2;

  // JL: if change in z, y is below tolerance, give zero response
  if (abs(z - newz) <= tolerance || abs(y - newy) <= tolerance) {
    response = 0;
  }

  return response;
}

// Accelerometer & Speaker routines
// JL: SOS signal
void sos(int oldz, int oldy) {  // NOT REASONABLE ANYMORE BECAUSE OF BLOCKING

  int helmStatus = 0;
  // NOTE_CS4 is the error sound
  int sig = NOTE;
  int pause = 500;
  int slen = 1000;

  while (helmStatus == 0) {

    buzz(melodyPin, sig, 1000);
    delay(pause);

    helmStatus = checkStatus(oldz, oldy, 1);
    
  }
  
}


// JL: buzz is the main function to output single sounds
void buzz(int targetPin, long frequency, long length) {
  //digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  //digitalWrite(13, LOW);

}




int k = 0;

// the loop function runs over and over again forever

// JL: within loop() integrate accelerometer and speaker in a non-blocking way
// JL: reasonable to use state controls for speaker:
// JL: states should be
// - standby                  : 0
// - waiting (buzz pending)   : 1
// - playing buzz             : 2
// JL: waiting must be according to set time
// JL: standby is entirely passive
// JL: playing buzz simply plays something
// more documentation in function below

unsigned int updateSpeakerState(unsigned long state_time, unsigned long deltatime, unsigned int s) {
  // FUNCTION TO CONTROL THE SPEAKER FUNCTIONS
  // Speaker states work as describes 0-2 above
  // If speaker is in state 0, it remains in state 0 (standby)
  // Outside accelerometer functions can activate state 1 (prepare for buzz)
  // In state 1, a waiting time counts down until the next buzz
  // When countdown is done, speaker moves to state 2 (emit buzz)
  // In state 2 the speaker emits a buzz, then checks helmet position for movement.
  // If helmet is stationary, return to state 1 (wait for next buzz)
  // If helmet is moving, no SOS is needed and speaker returns to state 0
  
  sojournTime = state_time + deltatime;

  // reset waiting time to prevent overflows
  if (sojournTime > 100000) {
    sojournTime = 0;
  }
  
  if (s == 0) {
    return 0;
  }
  else if (s == 1) {
    
    if (sojournTime > 500) {
      sojournTime = 0;
      return 2;
    }
    else {
      return 1;
    }
  }
  else {
    buzz(melodyPin, NOTE, 1000);
    sojournTime = 0;
    // finally check whether to change status
    int helmetstat = checkStatus(z2, y2);

    if (helmetstat == 0) {
      return 1;
    }
    else {
      return 0;
    }
  }
  
}



// speaker unit test
void speakerTest() {

  // verify that speaker stays in state 0
  speakerState = 0;
  speakerState = updateSpeakerState(1, 1, speakerState);
  assert(speakerState == 0);

  // verify that waiting for buzz works
  int z2 = 100;
  int y2 = 100;
  speakerState = 1;
  speakerState = updateSpeakerState(10, 1, speakerState);
  assert(speakerState == 1);

  speakerState = updateSpeakerState(499, 10, speakerState);
  assert(speakerState == 2);

  speakerState = updateSpeakerState(0, 1, speakerState);
  assert(speakerState == 1);
}


void loop() {

  unsigned long currentTime = millis();

  // JL: speaker and accelerometer logic below
  
  speakerState = updateSpeakerState(sojournTime, currentTime - prevTimeAcc, speakerState);

  if (currentTime - prevTimeAcc >= eventInterval && mittaus)  {
    x = analogRead(0); // read analog input analogPin 0
    y = analogRead(1); // read analog input analogPin 1
    z = analogRead(2); // read analog input analogPin 2
    Serial.print("accelerations are x, y, z: ");
    Serial.print(x, DEC); // print acceleration in the X axis
    Serial.print(" "); // prints a space between the numbers
    Serial.print(y, DEC); // print acceleration in the Y axis
    Serial.print(" "); // prints a space between the numbers
    Serial.println(z, DEC); // print acceleration in the Z axis
    prevTimeAcc = currentTime;     
                            
  }
  
  if ((z > 600 || y > 600) && mittaus)  {
     unsigned long iskuaika = currentTime;
     mittaus = false;
     valiaika = currentTime;
     Serial.print("isku havaittu");
     mittaus3 = true;
   }     
        
  if ((currentTime - valiaika >= 3000) && !mittaus && mittaus2  ) {
     z2 = analogRead(2); 
     y2 = analogRead(1);  // mittaa kiihtyvydeen uudestaan
     Serial.println("1. mittaus iskun jlk");
     mittaus2 = false;
     
  }        
  if ((currentTime - valiaika >= 5000) && !mittaus && !mittaus2) {
     z3 = analogRead(2);
     y3 = analogRead(1);  // mittaa kiihtyvyyden taas
     Serial.println("2. mittaus iskun jlk");
     mittaus = true;
     mittaus2 = true;
     x = analogRead(0); // read analog input pin 0
     y = analogRead(1); // read analog input pin 1
     z = analogRead(2); // read analog input pin 1
     valiaika2 = currentTime;
     
     if ((z3 == z2 || y2 == y3) && currentTime - valiaika2 < 30000) { 
       //Serial.print("SOS");       // jos kypärä jäänyt paikoilleen niin kertoo raspille: soita avunhuuto

       // JL: introduced sos sound 
       // must change to non-blocking
      // sos(z2, y2);
      // set the speaker to buzzing state instead of calling function
      speakerState = 1;
      sojournTime = 0;
       
      mittaus3 = false;
      
     previousTime = currentTime;
    }}

  prevTimeAcc = currentTime;
  
  if(Serial.available() > 0){
    String data = Serial.readStringUntil('\n');
    String command = data.substring(0, data.indexOf(" "));
    //Serial.print("You sent me: ");
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
      for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++){
        leds[i] = CRGB(0, 255, 0);
      }
      //previousTime = millis();
    } else if(command == "LEFT"){
      prepareLights("LEFT");
      for(int i = 0; i < NUM_LEDS / 2; i++) {
        leds[i] = CRGB(0, 255, 0);
      }
    } else if (command == "WEATHER" || command == "TIME") {
      resetOLED();
      writeOLED(data);  // NOTE: input is DATA, and not COMMAND
    } else if(command == "SCREEN") {
      if(!screenUp){
        //Serial.println("PUTTING SCR UP!");
        for(; screenPos < screenUpPos; screenPos++){
          screenServo.write(screenPos);
          delay(15);
        }
      } else {
        //This actually goes down
        //Serial.println("PUTTING SCR DOWN!");
        for(; screenPos > screenDownPos; screenPos--){
          screenServo.write(screenPos);
          delay(15);
        }
      }
      screenUp = !screenUp;
    }
  }

  if(millis() - blinkStart < blinkTime){  // WARNING: may cause LCD screen bugs? //JL: I have forgotten why I wrote this warning
    if (state == "RIGHT"){
      if(millis() - previousTime >= blinkInterval){
        ////Serial.println("BLINKING");
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
