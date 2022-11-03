### 20.10.2022
### First demo with a serial connection from a computer with Python to an Arduino.
### Sends a single integer to the Arduino, which processes it and returns it.
### How to run:
### 1. Connect Arduino to computer
### 2. Load "Arduino-receive-num-frompy" to the Arduino
### 3a. You can run "py-ardu serial-demo.py" from command line by:
### $ python py-ardu-serial-demo.py
### commandline argument: --num K , if you want to send number K
### 3b. Alternative: you can import the python function "send numeric" in another script.
### The send_numeric(k) detects the Arduino automatically and sends k over serial.

import serial
import serial.tools.list_ports
import time

# function to detect Arduino port
def detect_ard_port():
    # Automatically returns the port of a connected Arduino
    
    ports = list(serial.tools.list_ports.comports())
    
    # some flexibility in case multiple Arduinos, but selects the first one found
    arduino_ports = []
    for port in ports:
        
        if ("Arduino" in port.manufacturer):
            arduino_ports.append(port.description)
        
    if not arduino_ports:
        print("No Arduino found.")
        return 0
    
    if len(arduino_ports) > 1:
        print("Multiple Arduinos found.")
    
    return arduino_ports[0]

# serial comms
def send_numeric(num):
    ## not sure how to do without encoding
    
    port = '/dev/' + detect_ard_port()
    # baudrate must correspond to Arduino Serial port
    ## not sure how to select timeout
    arduino = serial.Serial(port=port, baudrate=115200, timeout=2)
    time.sleep(1)
    
    # like a socket, send as bytes
    arduino.write(bytes(str(num), 'utf-8'))
    
    ## must wait until receive data
    ## NOTE: readline() is NECESSARY, it reads until '/n' is found
    ## the function read() only reads in some set of bytes (maybe only 1?)
    data = arduino.readline();
    print(data.decode())  # decode to turn into Python string

if __name__ == "__main__":
    import argparse
    
    parser = argparse.ArgumentParser()
    parser.add_argument("--num", help="integer to send to Arduino", default=5, type=int)
    args = parser.parse_args()
    
    num = args.num
    print(detect_ard_port())
    print("Port detected. Can connect with Arduino.")
    print("Sending number " + str(num))
    
    send_numeric(num=num)
    
