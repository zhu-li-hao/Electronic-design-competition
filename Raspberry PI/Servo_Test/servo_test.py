#!/usr/bin/python3.9
# coding: utf-8

import RPi.GPIO as GPIO
import time

pin = 12

GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)
GPIO.setup(pin,GPIO.OUT) # pin为你所接的数据输出GPIO引脚。
p = GPIO.PWM(pin,50) # 在pin脚上产生频率为50HZ的PWM信号。
p.start(0) # 在引脚上设置一个初始PWM信号。

#while True:
p.ChangeDutyCycle(12) # 通过输入不同的“占空比值”来观察舵机的位置变化，这里的“占空比”是3%。（角度：0°）
time.sleep(1)
p.ChangeDutyCycle(9.7) # 通过输入不同的“占空比值”来观察舵机的位置变化，这里的“占空比”是3%。（角度：30°）
time.sleep(1)
p.ChangeDutyCycle(0) # 通过输入不同的“占空比值”来观察舵机的位置变化，这里的“占空比”是3%。（角度：0°）
time.sleep(1)

p.stop() # 停止PWM。
GPIO.cleanup() # 清理GPIO。
