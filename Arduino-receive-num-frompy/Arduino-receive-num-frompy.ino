// 20.10.2022
// First demo with a serial connection from a computer with Python to an Arduino.
// This Arduino code receives a number from port 115200 and returns the number + 10.
// How to run:
// 1. Connect Arduino to computer
// 2. Load this file to the Arduino
// 3a. You can run "py-ardu serial-demo.py" from command line by:
// $ python py-ardu-serial-demo.py
// commandline argument: --num K , if you want to send number K
// 3b. Alternative: you can import the python function "send numeric" in another script.
// The send_numeric(k) detects the Arduino automatically and sends k over serial.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(2);  // long timeout to minimize errors
}

void loop() {
  // put your main code here, to run repeatedly:
  while (!Serial.available()) {
  } // wait until data received
  String tmp = Serial.readString(); // read raw data
  int num = tmp.toInt() + 10;  // interpret as int, add 10
  Serial.println(num);  // necessary to println, just print() does not use a '/n'
  

}
