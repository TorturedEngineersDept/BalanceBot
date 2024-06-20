#!/usr/bin/python3

"""
Turns left for one second
Turns right for one second

enum KeyDirection
{
    FORWARD = 0,
    BACKWARD = 1,
    LEFT = 2,
    RIGHT = 3,
    STOP = 4
};


Alternates
"""

import serial
import time
import threading

ser = serial.Serial("/dev/ttyUSB0", 115200)
# ser = serial.Serial("COM4", 115200)
 
time.sleep(1)

def turnRight():
    ser.write((0b011).to_bytes(1, 'big'))

def turnLeft():
    ser.write((0b010).to_bytes(1, 'big'))

def stop():
    ser.write((0b100).to_bytes(1, 'big'))

def forward():
    ser.write((0b000).to_bytes(1, 'big'))

def backward():
    ser.write((0b001).to_bytes(1, 'big'))

def readSerial():
    while True:
        print(ser.readline().decode('utf-8').strip())

def controlMovement():
    while True:
        forward()
        time.sleep(1)
        stop()
        time.sleep(1)
        backward()
        time.sleep(1)
        stop()
        time.sleep(1)
        turnRight()
        time.sleep(1)
        stop()
        time.sleep(1)
        turnLeft()
        time.sleep(1)
        stop()
        time.sleep(1)
try:
    t1 = threading.Thread(target=readSerial, daemon=True).start()
    t2 = threading.Thread(target=controlMovement, daemon=True).start()

    while True:
        time.sleep(1)
except KeyboardInterrupt:
    if ser != None:
        ser.close()