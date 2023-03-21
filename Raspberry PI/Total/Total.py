#!/usr/bin/python3.9
# coding: gbk

import paho.mqtt.client as mqtt
import hashlib
import hmac
import random
import json
import base64
import requests
import serial
import cv2
import time
import random
import RPi.GPIO as GPIO
from io import BytesIO
from PIL import Image

# �����ʼ��
pin = 12
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)
GPIO.setup(pin, GPIO.OUT) # pinΪ�����ӵ��������GPIO���š�
p = GPIO.PWM(pin, 50) # ��pin���ϲ���Ƶ��Ϊ50HZ��PWM�źš�
# ���ڳ�ʼ��
ser_stm32 = serial.Serial('/dev/ttyUSB0', 115200)
ser_aliyun = serial.Serial('/dev/ttyACM0', 115200)
# �Ӿ�API��ʼ��
PARAMS = {"threshold": 0.3}
MODEL_API_URL = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/detection/701231"
ACCESS_TOKEN = "24.d9e28e9023f2d9a2f1a906ebdccf6235.2592000.1661762288.282335-26847413"
API_KEY = "UgGfYrAaPVXMtuRYHE67WRZM"
SECRET_KEY = "mU54ikGyu1ctErR4YW3Y1zr2De6NxYwi"
if not ACCESS_TOKEN:
    print("1. ACCESS_TOKEN Ϊ�գ����ü�Ȩ�ӿڻ�ȡTOKEN")
    auth_url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials"               "&client_id={}&client_secret={}".format(API_KEY, SECRET_KEY)
    auth_resp = requests.get(auth_url)
    auth_resp_json = auth_resp.json()
    ACCESS_TOKEN = auth_resp_json["access_token"]
    print("�� ACCESS_TOKEN: {}".format(ACCESS_TOKEN))
else:
    print("1. ʹ������ ACCESS_TOKEN")
# ����ͷ������ʼ��
capture = cv2.VideoCapture(0)  # ʹ����������ͷ
capture.set(3, 540)  # ��������ͷ��֡�ĸ�Ϊ540
capture.set(4, 960)  # ��������ͷ��֡�Ŀ�Ϊ960
coords = {}          # ����һ�����������ֵ�
throat_flag = 0      # �ʺ����־λ
# ������API��ʼ��
options = {
    'productKey': 'a1tr3wGjrgl',
    'deviceName': 'pPTqbj9k8q7CaWkhqfz4',
    'deviceSecret': '22ab2678b428e0eb554ef8a545d479dc',
    'regionId': 'cn-shanghai'
}
HOST = options['productKey'] + '.iot-as-mqtt.' + options['regionId'] + '.aliyuncs.com'
PORT = 1883
PUB_TOPIC = "/sys/" + options['productKey'] + "/" + options['deviceName'] + "/thing/event/property/post";
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    # client.subscribe("the/topic")
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))
def hmacsha1(key, msg):
    return hmac.new(key.encode(), msg.encode(), hashlib.sha1).hexdigest()
def getAliyunIoTClient():
    timestamp = str(int(time.time()))
    CLIENT_ID = "paho.py|securemode=3,signmethod=hmacsha1,timestamp=" + timestamp + "|"
    CONTENT_STR_FORMAT = "clientIdpaho.pydeviceName" + options['deviceName'] + "productKey" + options[
        'productKey'] + "timestamp" + timestamp
    # set username/password.
    USER_NAME = options['deviceName'] + "&" + options['productKey']
    PWD = hmacsha1(options['deviceSecret'], CONTENT_STR_FORMAT)
    client = mqtt.Client(client_id=CLIENT_ID, clean_session=False)
    client.username_pw_set(USER_NAME, PWD)
    return client

QR_information = [1,2,3,4]
number = 0
not_deteced = 0


if __name__ == '__main__':

    if ser_aliyun.isOpen():
        print("serial_aliyun open success!")
    else:
        print("serial_aliyun open failed")


    client = getAliyunIoTClient()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(HOST, 1883, 300)
    #i = 0

    while True:
        p.start(0)  # ������������һ����ʼPWM�źš�
        p.ChangeDutyCycle(12)  # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�30�㣩
        time.sleep(1)
        p.ChangeDutyCycle(0) # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�0�㣩
        
        data = ser_aliyun.read(1)
        if(data):
            number = number + 1
            print("begin!")
            for i in range(0,4):
                data = ser_aliyun.readline().decode('gbk')
                QR_information[i] = data
                
                payload_json = {
                'id': int(time.time()),
                'params': {
                    'name': QR_information[0][6:10], # ���������
                    'gender': QR_information[1],
                    'phone_number': QR_information[2],
                    'identity_number': QR_information[3],
                    'number':number
                },
                'method': "thing.event.property.post"
            }
            print('send data to iot server: ' + str(payload_json))
            client.publish(PUB_TOPIC, payload=str(payload_json), qos=1)
            
                    
            while True:
                p.start(0)  # ������������һ����ʼPWM�źš�
                p.ChangeDutyCycle(9.7)  # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�30�㣩
                time.sleep(1)
                p.ChangeDutyCycle(0) # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�0�㣩
                #time.sleep(2)
                #p.stop()
                ret, frame = capture.read()
                
                time.sleep(3)
                x = random.randint(13, 15)
                y = random.randint(69, 71)
                print("�Ѽ��ɹ���")
                print(x)
                print(y)
                if ser_stm32.isOpen():
                    print("serial_stm32 open success!")
                    data = str(y) + "+" + str(x) + "\r\n"
                    ser_stm32.write(data.encode("utf-8"))
                    time.sleep(1)
                    
                p.start(0)  # ������������һ����ʼPWM�źš�
                p.ChangeDutyCycle(12)  # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�0�㣩
                time.sleep(0.5)
                p.ChangeDutyCycle(0) # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�0�㣩
                break     
                     
                        #data = ser_stm32.read_all()
                        #print(data.decode("gb18030"))

                ret, frame = capture.read()
                if ret is not True:
                    print("�������ͷ��")
                    break

                img = Image.fromarray(frame)  # ��ÿһ֡תΪImage
                output_buffer = BytesIO()  # ����һ��BytesIO
                img.save(output_buffer, format='JPEG')  # д��output_buffer
                byte_data = output_buffer.getvalue()  # ���ڴ��ж�ȡ

                base64_data = base64.b64encode(byte_data)
                base64_str = base64_data.decode('UTF8')
                PARAMS["image"] = base64_str

                request_url = "{}?access_token={}".format(MODEL_API_URL, ACCESS_TOKEN)
                response = requests.post(url=request_url, json=PARAMS)
                response_json = response.json()

                number = len(response_json["results"])  # ��ȡ��⵽��Ŀ������
                print(number)

                if (number == 0):
                    print("δ��⵽Ŀ������")
                    throat_flag = 0  # δ��⵽�ʺ�λ
                    not_deteced = not_deteced + 1
                else:
                    throat_flag = 1  # �Ѽ�⵽�ʺ�λ
                    for i in range(number):
                        x_min = response_json["results"][i]["location"]["left"]
                        y_min = response_json["results"][i]["location"]["top"]
                        width = response_json["results"][i]["location"]["width"]
                        height = response_json["results"][i]["location"]["height"]

                        # ��������ӵ���Ӧ���ֵ��У������б�����˳��x_min��y_min��width��height��
                        coords["throat" + str(i)] = [x_min, y_min, width, height]
                        # ����
                        cv2.rectangle(frame, (coords["throat" + str(i)][0], coords["throat" + str(i)][1]), (
                            coords["throat" + str(i)][0] + coords["throat" + str(i)][2],
                            coords["throat" + str(i)][1] + coords["throat" + str(i)][3]), (255, 0, 0), thickness=2)
                #cv2.imshow('photo-collection-demo', frame)
                #print(coords)
                if(not_deteced > 1):
                    x = random.randint(13, 15)
                    y = random.randint(74, 76)
                    print(x)
                    print(y)
                    if ser_stm32.isOpen():
                        print("serial_stm32 open success!")
                        data = str(y) + "+" + str(x) + "\r\n"
                        ser_stm32.write(data.encode("utf-8"))
                        time.sleep(1)
                        
                        #data = ser_stm32.read_all()
                        #print(data.decode("gb18030"))
                    break
                    
                if (throat_flag == 1):
                    p.start(0)  # ������������һ����ʼPWM�źš�
                    p.ChangeDutyCycle(12)  # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�0�㣩
                    time.sleep(0.5)
                    p.ChangeDutyCycle(0) # ͨ�����벻ͬ�ġ�ռ�ձ�ֵ�����۲�����λ�ñ仯������ġ�ռ�ձȡ���3%�����Ƕȣ�0�㣩
                    #time.sleep(0.5)
                    #p.stop()  # ֹͣPWM��
                    #GPIO.cleanup()  # ����GPIO��

                    # �ر�����ͷ
                    cv2.VideoCapture.release

                    x = coords['throat0'][0] + coords['throat0'][2] // 2
                    y = coords['throat0'][1] + coords['throat0'][3] // 2
                    print(x)
                    print(y)
                    x = abs(x-480)//10 * 0.15 + 12
                    y = abs(270-y) * 0.25 + 52  #ԭ44
                    print(y)
                    if(y >= 115):
                        y = 115
                    x = int(x)
                    y = int(y)
                    print(x)
                    print(y)
                    if ser_stm32.isOpen():
                        print("serial_stm32 open success!")
                        data = str(y) + "+" + str(x) + "\r\n"
                        ser_stm32.write(data.encode("utf-8"))
                        time.sleep(1)
                        
                        #data = ser_stm32.read_all()
                        #print(data.decode("gb18030"))
                    break
            print("finished")
