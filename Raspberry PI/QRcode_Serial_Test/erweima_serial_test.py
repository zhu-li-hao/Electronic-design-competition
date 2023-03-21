#!/usr/bin/python3.9
# coding: utf-8

import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200)

xinxi = [1,2,3,4]
if ser.isOpen():
    i = 0
    print("serial open success!")
    #ser.write("hi\r\n".encode("utf-8"))
    while True:
        
        time.sleep(0.5)
        #data = ser.read_all()
        data = ser.readline().decode("gbk")
        if(data != 0):
            print(data)
            xinxi[i] = data
            i = i +1
            #print(type(data))
            #print(data[7:10]) # 7~10打印姓名
            #print(data[10:21]) # 11~22打印电话号码
            #print(data[21:39]) # 23~40打印身份证号码
        print(xinxi)
else:
    print("serial open failed")
