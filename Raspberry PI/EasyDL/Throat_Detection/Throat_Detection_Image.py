#!/usr/bin/python3.9
# coding: utf-8

"""
EasyDL 物体检测 调用模型公有云API Python3实现
"""

# import json
import base64
import requests
import cv2
import serial
import time

"""
使用 requests 库发送请求
使用 pip（或者 pip3）检查我的 python3 环境是否安装了该库，执行命令
  pip freeze | grep requests
若返回值为空，则安装该库
  pip install requests
"""

# 初始化串口信息
ser = serial.Serial('/dev/ttyUSB1', 115200)

# 目标图片的 本地文件路径，支持jpg/png/bmp格式
IMAGE_FILEPATH = "./WIN_20220725_14_54_00_Pro.jpg"

# 可选的请求参数
# threshold: 默认值为建议阈值，请在 我的模型-模型效果-完整评估结果-详细评估 查看建议阈值
PARAMS = {"threshold": 0.3}

# 服务详情 中的 接口地址
MODEL_API_URL = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/detection/701231"

# 调用 API 需要 ACCESS_TOKEN。若已有 ACCESS_TOKEN 则于下方填入该字符串
# 否则，留空 ACCESS_TOKEN，于下方填入 该模型部署的 API_KEY 以及 SECRET_KEY，会自动申请并显示新 ACCESS_TOKEN
ACCESS_TOKEN = "24.ec1912085d02df981ffb683357455517.2592000.1661876288.282335-26847413"
API_KEY = "UgGfYrAaPVXMtuRYHE67WRZM"
SECRET_KEY = "mU54ikGyu1ctErR4YW3Y1zr2De6NxYwi"


print("1. 读取目标图片 '{}'".format(IMAGE_FILEPATH))
with open(IMAGE_FILEPATH, 'rb') as f:
    base64_data = base64.b64encode(f.read())
    base64_str = base64_data.decode('UTF8')
print("将 BASE64 编码后图片的字符串填入 PARAMS 的 'image' 字段")
PARAMS["image"] = base64_str


if not ACCESS_TOKEN:
    print("2. ACCESS_TOKEN 为空，调用鉴权接口获取TOKEN")
    auth_url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials"               "&client_id={}&client_secret={}".format(API_KEY, SECRET_KEY)
    auth_resp = requests.get(auth_url)
    auth_resp_json = auth_resp.json()
    ACCESS_TOKEN = auth_resp_json["access_token"]
    print("新 ACCESS_TOKEN: {}".format(ACCESS_TOKEN))
else:
    print("2. 使用已有 ACCESS_TOKEN")


print("3. 向模型接口 'MODEL_API_URL' 发送请求")
request_url = "{}?access_token={}".format(MODEL_API_URL, ACCESS_TOKEN)
response = requests.post(url=request_url, json=PARAMS)
response_json = response.json()

# opencv加载显示图像
img = cv2.imread(IMAGE_FILEPATH)
number = len(response_json["results"])  # 获取检测到的目标数量
coords = {}  # 定义一个存放坐标的字典

for i in range(number):
    x_min = response_json["results"][i]["location"]["left"]
    y_min = response_json["results"][i]["location"]["top"]
    width = response_json["results"][i]["location"]["width"]
    height = response_json["results"][i]["location"]["height"]

    # 将坐标添加到对应的字典中
    # 坐标列表排列顺序（x_min，y_min，width，height）
    coords["throat" + str(i)] = [x_min, y_min, width, height]
    # 画框
    cv2.rectangle(img, (coords["throat" + str(i)][0], coords["throat" + str(i)][1]), (coords["throat" + str(i)][0] + coords["throat" + str(i)][2], coords["throat" + str(i)][1] + coords["throat" + str(i)][3]), (255, 0, 0), thickness=2)

print(coords)

x = coords['throat0'][0] + coords['throat0'][2]//2
y = coords['throat0'][1] + coords['throat0'][3]//2
x = (x-960)//10
y = (540-y)//10
print(x)
print(y)
if ser.isOpen():
    print("serial open success!")
    data = str(x) + "+" + str(y) + "\r\n"
    ser.write(data.encode("utf-8"))
    time.sleep(0.5)
    #data = ser.read_all()
    #print(data.decode('gbk'))

img = cv2.resize(img, (960,540))  # 图像缩放
cv2.imshow('demo', img)
cv2.waitKey(0)
cv2.destroyAllWindows()
