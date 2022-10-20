#!/usr/bin/env python3

# NOTE: this example requires PyAudio because it uses the Microphone class
import time

import speech_recognition as sr

def visiiri(text):
    sliced = text.split(' ')
    for word in sliced:
        if word in ["open", "op", "pop", ""]:
            return "OPEN"
        elif word in ["close", "down"]:
            return "CLOSE"
        else:
            return "UNKNOWN"

r = sr.Recognizer()
while True:
    try:
        with sr.Microphone() as source:
            # read the audio data from the default microphone
            audio_data = r.record(source, duration=5)
            print("Recognizing...")
            # convert speech to text
            text = r.recognize_google(audio_data)
            print(text)
            print(visiiri(text))
    except:
        print('Error')
    # time.sleep(1.5)