/*
 * Jonas notes:
 * O. Pajunen developed main code for the accelerometer functions.
 * I leave O. Pajunen's comments unedited.
 * My own comments are denoted by "JL:"
 * My contributions are only to change the timers and SOS sounds, 
 * and to make an Arduino editor .ino file for the program.
 */

/*
  Arduino Mario Bros Tunes
  With Piezo Buzzer and PWM

  Connect the positive side of the Buzzer to pin 3,
  then the negative side to a 1k ohm resistor. Connect
  the other side of the 1 k ohm resistor to
  ground(GND) pin on the Arduino.

  by: Dipto Pratyaksa
  https://create.arduino.cc/projecthub/jrance/super-mario-theme-song-w-piezo-buzzer-and-arduino-1cc2e4
  last updated: 31/3/13
*/

/*************************************************
 * Public Constants
 *************************************************/

// JL: music tones for O. Pajunen's mario theme

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define melodyPin 3
//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
//Underworld melody
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};
//Underwolrd tempo
int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

// JL: I define an SOS signal below based on OP's music
int SOS_tempo[] = {
  18, 18, 18,
  2,
  1, 1, 1,
  2,
  18, 18, 18
};

int SOS_signal[] = {
  NOTE_E6, NOTE_E6, NOTE_E6,
  0,
  NOTE_CS4, NOTE_CS4, NOTE_CS4,
  0,
  NOTE_E6, NOTE_E6, NOTE_E6,
};

int x, y, z, z2, y2, z3, y3 ; 
const unsigned long eventInterval = 100;
unsigned long previousTime = 0;
unsigned long valiaika = 0;
bool mittaus = true;
bool mittaus2 = true;
bool mittaus3 = true;
unsigned long valiaika2 = 0;

void setup()

{
  Serial.begin(9600); // sets the serial port to 9600
  pinMode(3, OUTPUT);//buzzer
  pinMode(13, OUTPUT);//led indicator when singing a note

  // JL: debugging code below, ignore
  //z = analogRead(2);
  //y = analogRead(1);
  //sos(z, y);

}

void loop()
{
  unsigned long currentTime = millis();
   
  if (currentTime - previousTime >= eventInterval && mittaus)  {
    x = analogRead(0); // read analog input pin 0
    y = analogRead(1); // read analog input pin 1
    z = analogRead(2); // read analog input pin 1
    Serial.print("accelerations are x, y, z: ");
    Serial.print(x, DEC); // print acceleration in the X axis
    Serial.print(" "); // prints a space between the numbers
    Serial.print(y, DEC); // print acceleration in the Y axis
    Serial.print(" "); // prints a space between the numbers
    Serial.println(z, DEC); // print acceleration in the Z axis
    previousTime = currentTime;     
                            
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
       Serial.print("SOS");       // jos kypärä jäänyt paikoilleen niin kertoo raspille: soita avunhuuto

       // JL: commented out OP's tunes
       //sing(1);
       //sing(1);
       //sing(2);

       // JL: introduced sos sound
      sos(z2, y2);
       
      mittaus3 = false;
      
     previousTime = currentTime;
    }}
}


// JL: below is OP's code for playing music
// JL: music is cool, but I add an SOS signal below

int song = 0;

void sing(int s) {
  // iterate over the notes of the melody:
  song = s;
  if (song == 2) {
    Serial.println(" 'Underworld Theme'");
    int size = sizeof(underworld_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / underworld_tempo[thisNote];

      buzz(melodyPin, underworld_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }

  } else {

    Serial.println(" 'Mario Theme'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];

      buzz(melodyPin, melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }
  }
}

// JL: SOS signal
void sos(int oldz, int oldy) {

  int helmStatus = 0;
  // NOTE_CS4 is the error sound
  int sig = NOTE_CS4;
  int pause = 500;
  int slen = 1000;

  while (helmStatus == 0) {

    buzz(melodyPin, sig, 1000);
    delay(pause);

    helmStatus = checkStatus(oldz, oldy);
    
  }
  
  // JL: modeled after OP's song code
  // JL: DEPRECATED CODE
  /*
  int len = sizeof(SOS_signal);

  for (int thisNote = 0; thisNote < len; thisNote++) {

    // JL: note duration based on tempo
    int noteDuration = 1000 / SOS_tempo[thisNote];

    buzz(melodyPin, SOS_signal[thisNote], noteDuration);

    // JL: OP suggested note duration + 30% for between note pause
    int pauseBetweenNotes = 1000;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    buzz(melodyPin, 0, noteDuration);
  }
  */
}



int checkStatus(int z, int y) {
  // JL: function to verify whether helmet has moved since an impulse
  // returns 1 if helmet has moved by more than tolerance (int)
  // returns 0 otherwise

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


// JL: buzz is the main function to output single sounds
void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
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
  digitalWrite(13, LOW);

}
