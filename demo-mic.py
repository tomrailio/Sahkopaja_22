#!/usr/bin/env python3

# NOTE: uses pocketsphinx but if google API works then it can be changed
# pocketsphinx installation
# also uses pyaudio

# run this script by executing "python3 demo-mic.py"

import time
import speech_recognition as sr
import serial

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=6)
ser.reset_input_buffer()
exit = False

WORD_LIST = []
KEYWORDS = [("left", 0.05), ("right", 0.05), ("stop", 0.01), ("dummy", 1.00)]


def visiiri(text):
    sliced = text.split(' ')
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
            ser.write(translate)

    except:
        print("Error")

## python script to listen in background
if __name__ == "__main__":
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

    