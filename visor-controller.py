### WIP pseudocode for visor control
### REQUIREMENTS: - PocketSphinx (or alternative speech rec. engine)
### - Stepper motor control (either with Rasp or Arduino)
### TODO: Import pocket sphinx (or other) and integrate 

import serial
import serial.tools.list_ports
import time
from sys import platform

# function to detect Arduino port
def detect_ard_port():
    # Automatically returns the port of a connected Arduino
    # This is useful because different operating systems have different names
    # added platform compatibility
    
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
    
    if platform.startswith("linux"):  ## some systems are "linux2" or "linux3"
        return '/dev/' + arduino_ports[0]
    else
        return arduino_ports[0]

def send2ard(data):
    # serial baudrate 115200
    br = 115200
    # br = 9600
    # br = 38400
    # I don't even understand what this means
    
    port = detect_ard_port()
    arduino = serial.Serial(port=port, baudrate=br, timeout=2)
    
    # must have active Arduino waiting
    arduino.write(data)  # TODO: make sure data transfer works
    
    # consider waiting for Arduino response?
    # response = arduino.readline()
    # return response ## controller should decide what to do

def LRcontroller(signal):
    ## project at a too early stage to work out how this should work
    
    if "LEFT" in signal:
        res = send2ard(0)
    elif "RIGHT" in signal:
        res = send2ard(1)
    
    

def visor(signal):
    # signal should be in format outputted by a speech rec system
    
    response = ""
    
    if "UP" in signal.upper():
        response = "UP"
    elif "DOWN" in signal.lower():
        response = "DOWN"
    else
        pass

    ## TODO: response must be processed by some control function
    ## and result in motor movement
    ## currently it is not clear how to do this
    return response
    


