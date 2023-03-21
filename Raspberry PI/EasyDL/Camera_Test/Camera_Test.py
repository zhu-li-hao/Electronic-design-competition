#!/usr/bin/python3.9
# coding: utf-8

import cv2

cap=cv2.VideoCapture(0)

while(1):
    ret ,frame = cap.read()
    cv2.imshow("capture", frame)
    k=cv2.waitKey(1)
    if k==27:           #按下ESC退出窗口
        break
    
cap.release()
