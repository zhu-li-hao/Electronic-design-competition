#!/usr/bin/python3.9
# coding: utf-8

import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200)

if ser.isOpen():
    print("serial open success!")
    
    #while True:
    data = ser.readline()#.decode('gbk')
    print(data)
    #print(data[7:10].decode('gbk'))
else:
    print("serial open failed")
