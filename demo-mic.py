#!/usr/bin/env python3

"""
# 1. DESCRIPTION
Python script for speech recognition and sending voice commands to Arduino controller.

# 2. PYTHON PACKAGE DEPENDENCIES
- Python 3.8
- PyAudio 0.2.11
- SpeechRecognition 3.8.1
- pyserial 3.5
- requests 2.22.0

# 3. DESCRIPTION OF FUNCTIONALITY
Once activated, script keeps running until it is shut down by the voice command "QUIT".
The script listens to certain keywords and communicates them to an Arduino controller over serial.
By default the speech recognition relies on Google software.

The following are valid voice commands:
- OPEN
- CLOSE
- LEFT
- RIGHT
- WEATHER
- TIME
- QUIT or EXIT

Voice command "WEATHER", the script downloads temperature data from openweathermap.org, an open API which needs an API key.
Voice command "TIME", the script uses Python functions to check current time (dep. on machine clock).
In the above two cases, the script also sends a data string via serial.

# 4. DESCRIPTION OF USAGE
The script is run using the terminal command:
$ python3 demo-mic.py <weather-api-key>
where the <weather-api-key> is replaced with a valid API key from openweathermap.org

After startup, the script runs until given the voice command "QUIT"/"EXIT", or until force closed from terminal/bash.

# 5. COMMENTS
Google speech recognition API can be replaced with a PocketSphinx-based solution if desired.
PocketSphinx can be locally deployed, so this circumvents heavy network usage.
On the other hand, the Open Weather API can not be accessed without an internet connection.

"""

import time
import speech_recognition as sr
import serial
import requests
import sys

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=2)  # LINUX SERIAL, IF RUNNING ON WINDOWS, VERIFY SERIAL PATH
ser.reset_input_buffer()
exit = False
weatherURL = "https://api.openweathermap.org/data/2.5/weather?q=Helsinki&appid="

WORD_LIST = []


def visiiri(text):
    sliced = text.split(' ')
    print("WHat did I say?")
    print(text)
    for word in sliced:
        if word in ["open", "op", "pop", ""]:
            return b"OPEN"
        elif word in ["close", "down"]:
            return b"CLOSE"
        elif word in ["quit", "exit"]:
            return b"QUIT"
        elif word in ["left"]:
            return b"LEFT"
        elif word in ["right"]:
            return b"RIGHT"
        elif word in ["weather", "whether", "wither"]:
            r = requests.get(url=weatherURL + sys.argv[1])
            data = r.json()
            temp = round(data["main"]["temp"] - 273.15, 1)
            print("Temperature?")
            print(temp)
            s = "WEATHER " + str(temp)
            return bytes(s, "utf-8")
        elif word in ["time", "thyme", "clock", "claw"]:
            t = time.localtime(time.time())
            t_str = str(t.tm_hour).zfill(2) + ":" + str(t.tm_min).zfill(2)
            print(t_str)
            s = "TIME " + t_str
            return bytes(s, "utf-8")
        elif word in ["screen", "scream", "green", "Queen", "queen"]:
            return b"SCREEN"
        else:
            return b"UNKNOWN"

# based on python speech_recognition background listening demo
def callback(recognizer, audio):
    print("CALLBACK!")
    try:
        text = recognizer.recognize_google(audio)
        #text = recognizer.recognize_sphinx(audio)

        translate = visiiri(text)
        if translate.decode() == 'QUIT':
            print("QUITTING")
            global exit
            exit = True
            # break
        if (translate.decode() != "UNKNOWN"):
            print("SENDING!")
            print(translate)
            ser.write(translate)

    except Exception as e:
        print("Error")
        print(e)

## python script to listen in background
if __name__ == "__main__":
    
    # visiiri("time")
        
    
    r = sr.Recognizer()
    m = sr.Microphone()
    
    with m as source:
        r.adjust_for_ambient_noise(source, duration = 1)
    
    #print(r.dynamic_energy_treshold)
    print(r.energy_threshold)
    
    stop_listening = r.listen_in_background(m, callback)
    # `stop_listening` is now a function that, when called, stops background listening
    

    while not exit:
        
        # wait 2 seconds
        time.sleep(2)
            
    
    # calling this function requests that the background listener stop listening
    stop_listening(wait_for_stop=False)

    
