#!/usr/bin/python3.9
# coding: utf-8

import json
import base64
import requests
import cv2
from io import BytesIO
from PIL import Image

"""
使用 requests 库发送请求
使用 pip（或者 pip3）检查我的 python3 环境是否安装了该库，执行命令
  pip freeze | grep requests
若返回值为空，则安装该库
  pip install requests
"""

# 可选的请求参数
# threshold: 默认值为建议阈值，请在 我的模型-模型效果-完整评估结果-详细评估 查看建议阈值
PARAMS = {"threshold": 0.3}

# 服务详情 中的 接口地址
MODEL_API_URL = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/detection/701231"

# 调用 API 需要 ACCESS_TOKEN。若已有 ACCESS_TOKEN 则于下方填入该字符串
# 否则，留空 ACCESS_TOKEN，于下方填入 该模型部署的 API_KEY 以及 SECRET_KEY，会自动申请并显示新 ACCESS_TOKEN
ACCESS_TOKEN = "24.d9e28e9023f2d9a2f1a906ebdccf6235.2592000.1661762288.282335-26847413"
API_KEY = "UgGfYrAaPVXMtuRYHE67WRZM"
SECRET_KEY = "mU54ikGyu1ctErR4YW3Y1zr2De6NxYwi"

if not ACCESS_TOKEN:
    print("1. ACCESS_TOKEN 为空，调用鉴权接口获取TOKEN")
    auth_url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials"               "&client_id={}&client_secret={}".format(API_KEY, SECRET_KEY)
    auth_resp = requests.get(auth_url)
    auth_resp_json = auth_resp.json()
    ACCESS_TOKEN = auth_resp_json["access_token"]
    print("新 ACCESS_TOKEN: {}".format(ACCESS_TOKEN))
else:
    print("1. 使用已有 ACCESS_TOKEN")


capture = cv2.VideoCapture(0)  # 使用外置摄像头
capture.set(3, 540)  # 设置摄像头的帧的高为640
capture.set(4, 960)  # 设置摄像头的帧的宽为480
coords = {}  # 定义一个存放坐标的字典

while True:
    ret, frame = capture.read()
    if ret is not True:
        print("请打开摄像头！")
        break

    img = Image.fromarray(frame)  # 将每一帧转为Image
    output_buffer = BytesIO()  # 创建一个BytesIO
    img.save(output_buffer, format='JPEG')  # 写入output_buffer
    byte_data = output_buffer.getvalue()  # 在内存中读取

    base64_data = base64.b64encode(byte_data)
    base64_str = base64_data.decode('UTF8')
    PARAMS["image"] = base64_str

    request_url = "{}?access_token={}".format(MODEL_API_URL, ACCESS_TOKEN)
    response = requests.post(url=request_url, json=PARAMS)
    response_json = response.json()

    number = len(response_json["results"])  # 获取检测到的目标数量

    if(number == 0):
        print("未检测到目标物体")

    for i in range(number):
        x_min = response_json["results"][i]["location"]["left"]
        y_min = response_json["results"][i]["location"]["top"]
        width = response_json["results"][i]["location"]["width"]
        height = response_json["results"][i]["location"]["height"]

        # 将坐标添加到对应的字典中
        # 坐标列表排列顺序（x_min，y_min，width，height）
        coords["throat" + str(i)] = [x_min, y_min, width, height]
        # 画框
        cv2.rectangle(frame, (coords["throat" + str(i)][0], coords["throat" + str(i)][1]), (
        coords["throat" + str(i)][0] + coords["throat" + str(i)][2],
        coords["throat" + str(i)][1] + coords["throat" + str(i)][3]), (255, 0, 0), thickness=2)
        print(coords)

    cv2.imshow('photo-collection-demo', frame)
    c = cv2.waitKey(1)
    # 按esc退出视频
    if c == 27:
        break
