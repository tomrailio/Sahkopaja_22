import serial
import time
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.reset_input_buffer()
    i = 0
    text = b"off\n"
    while True:
        if(i % 3 == 0):
            text = b"on\n"
        if(i % 5 == 0):
            text = b"off\n"
        ser.write(text)
        #line = ser.readline().decode('utf-8').rstrip()
        print("Text is")
        print(text)
        i += 1
        time.sleep(1)