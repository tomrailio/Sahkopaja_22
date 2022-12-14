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
        elif word in ["left"]:
            return b"LEFT"
        elif word in ["right"]:
            return b"RIGHT"
        else:
            return b"UNKNOWN"

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=6)
ser.reset_input_buffer()
#r = sr.Recognizer()

def callback(recognizer, audio):
    print("CALLBACK!")
    try:
        #text = recognizer.recognize_google(audio)
        text = recognizer.recognize_sphinx(audio)

        translate = visiiri(text)
        if translate.decode() == 'QUIT':
            print("QUITTING")
            # break
        print(translate)
        if (translate.decode() != "UNKNOWN"):
            print("SENDING!")
            ser.write(translate)

    except:
        print("Error")


r = sr.Recognizer()
m = sr.Microphone()

stop_listening = r.listen_in_background(m, callback)

exit = False

while not exit:
    print("LOOP")
    time.sleep(2)


"""
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
            if(translate.decode() != "UNKNOWN"):
                print("SENDING!")
                ser.write(translate)
            # time.sleep(1)
    except:
        print('Error')
    # time.sleep(1.5)
"""