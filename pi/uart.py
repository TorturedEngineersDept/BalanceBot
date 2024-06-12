#!/usr/bin/python3

import serial
import time

# ser = serial.Serial("/dev/ttyUSB0", 115200)
ser = serial.Serial("COM4", 115200)
 
time.sleep(1)

try:
    while True:
        print(ser.readline().decode('utf-8').strip()) 
        # 0b011 - Turn Right
        # 0b10000 - Speed = 16
        ser.write((0b01110000).to_bytes(1, 'big'))
except KeyboardInterrupt:
    if ser != None:
        ser.close()
