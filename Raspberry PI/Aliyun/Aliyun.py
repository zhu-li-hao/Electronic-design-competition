#!/usr/bin/python3.9
# coding: gbk
import paho.mqtt.client as mqtt
import time
import hashlib
import hmac
import random
import json
import serial

# 这个就是我们在阿里云注册产品和设备时的三元组啦
# 把我们自己对应的三元组填进去即可
options = {
    'productKey': 'a1tr3wGjrgl',
    'deviceName': 'pPTqbj9k8q7CaWkhqfz4',
    'deviceSecret': '22ab2678b428e0eb554ef8a545d479dc',
    'regionId': 'cn-shanghai'
}

HOST = options['productKey'] + '.iot-as-mqtt.' + options['regionId'] + '.aliyuncs.com'
PORT = 1883
PUB_TOPIC = "/sys/" + options['productKey'] + "/" + options['deviceName'] + "/thing/event/property/post";

ser = serial.Serial('/dev/ttyACM0', 115200)

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

#number = 0
QR_information = [1,2,3,4]

if __name__ == '__main__':
    if ser.isOpen():
        print("serial open success!")
    else:
        print("serial open failed")


    client = getAliyunIoTClient()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(HOST, 1883, 300)
    #i = 0

    while True:
        #data = ser.read(1)
        #print(data)
        if(ser.read(1)):
            print("begin!")
            for i in range(0,4):
                data = ser.readline().decode('gbk')
                QR_information[i] = data
                
                payload_json = {
                'id': int(time.time()),
                'params': {
                    'name': QR_information[0][6:10], # 检测者姓名
                    'gender': QR_information[1], #检测者性别
                    'phone_number': QR_information[2], #检测者电话号码
                    'identity_number': QR_information[3] #检测者身份证号
                },
                'method': "thing.event.property.post"
            }
            
            #data = ser_aliyun.readline().decode('gbk')
            
            print('send data to iot server: ' + str(payload_json))
            client.publish(PUB_TOPIC, payload=str(payload_json), qos=1)
        else:
            payload_json = {
            'id': int(time.time()),
            'params': {
                'number':0
            },
            'method': "thing.event.property.post"}
            print('send data to iot server: ' + str(payload_json))
            client.publish(PUB_TOPIC, payload=str(payload_json), qos=1)
    #client.loop_forever()
