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
#define NUM_LEDS 8
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
int serv_up = 25;
Servo theservo;

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
}

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

// the loop function runs over and over again forever
void loop() {
  if(Serial.available() > 0){
    String data = Serial.readStringUntil('\n');
    Serial.print("You sent me: ");
    if(data == "OPEN"){  // OPEN THE VISOR
      //digitalWrite(LED_BUILTIN, HIGH);
      //leds[0] = CRGB(255,0,0);

      theservo.write(serv_up);
    } else if(data == "CLOSE"){  // CLOSE THE VISOR
      //digitalWrite(LED_BUILTIN, LOW);
      //leds[0] = CRGB(0,0,0);

      theservo.write(serv_down);
    } else if(data == "RIGHT"){
      prepareLights("RIGHT");
      for (int i = 4; i < NUM_LEDS; i++){
        leds[i] = CRGB(0, 255, 0);
      }
      //previousTime = millis();
    } else if(data == "LEFT"){
      prepareLights("LEFT");
      for(int i = 0; i < NUM_LEDS / 2; i++) {
        leds[i] = CRGB(0, 255, 0);
      }
    }
  }


  if(millis() - blinkStart < blinkTime){
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
