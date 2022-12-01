// MINIMAL HELMET SERVO EXAMPLE
// TESTED WITH DK'S & OP'S HELMET

#include <Servo.h>

// Servo configs (VERIFY)
int servo_position = 120;
int serv_down = 120;
int serv_up = 160;
Servo theservo;

int SERV_PIN = 9;

void setup() {
  // put your setup code here, to run once:

  delay(500);

  // SERVO setup
  theservo.attach(SERV_PIN);
  theservo.write(0);

  delay(1500);

  Serial.println("servo_position: " + servo_position);

  delay(1500);

  theservo.write(70);

  //servo_position = k;

  Serial.println("servo_position: " + servo_position);


}

void loop() {
  // put your main code here, to run repeatedly:

  delay(10000);
}
