#!/usr/bin/env python3

# NOTE: this example requires PyAudio because it uses the Microphone class
import time
import serial

import speech_recognition as sr

def visiiri(text):
    sliced = text.split(' ')
    for word in sliced:
        if word in ["open", "op", "pop", ""]:
            return b"OPEN"
        elif word in ["close", "down"]:
            return b"CLOSE"
        elif word in ["quit", "exit"]:
            return b"QUIT"
        else:
            return b"UNKNOWN"

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=6)
ser.reset_input_buffer()
r = sr.Recognizer()

while True:
    try:
        with sr.Microphone() as source:
            # read the audio data from the default microphone
            audio_data = r.record(source, duration=5)
            print("Recognizing...")
            # convert speech to text
            text = r.recognize_google(audio_data)
            translate = visiiri(text)
            if translate.decode() == 'QUIT':
                print("QUITTING")
                break
            print(translate)
            ser.write(translate)
            # time.sleep(1)
    except:
        print('Error')
    # time.sleep(1.5)