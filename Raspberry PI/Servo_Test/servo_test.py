#!/usr/bin/python3.9
# coding: utf-8

import RPi.GPIO as GPIO
import time

pin = 12

GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)
GPIO.setup(pin,GPIO.OUT) # pinΪ�����ӵ��������GPIO���š�
p = GPIO.PWM(pin,50) # ��pin���ϲ���Ƶ��Ϊ50HZ��PWM�źš�
p.start(0) # ������������һ����ʼPWM�źš�

#while True:
p.ChangeDutyCycle(12) # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�0�㣩
time.sleep(1)
p.ChangeDutyCycle(9.7) # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�30�㣩
time.sleep(1)
p.ChangeDutyCycle(0) # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�0�㣩
time.sleep(1)

p.stop() # ֹͣPWM��
GPIO.cleanup() # ����GPIO��
