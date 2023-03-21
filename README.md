# Electronic-design-competition
2022年广东省电子设计竞赛参赛作品（基于机器视觉的核酸咽拭子机器人）

Electronic design competition:  
├─Others //此文件夹存放了咽喉检测模型训练的有关文件  
│  ├─Annotations //图片集标注后输出的xml文件  
│  ├─Images  
│  │  ├─test //咽喉检测模型的测试集  
│  │  └─train //咽喉检测模型的训练集     
│  └─venv //训练模型的环境配置文件  
│      └─Scripts  
├─Raspberry PI //此文件夹存放了本项目中由树莓派负责的代码  
│  ├─Aliyun  //二维码信息和检测结果上传至阿里云服务器   
│  ├─EasyDL  
│  │  ├─Camera_Test //相机测试代码  
│  │  ├─EasyDL_API //EasyDL的接口调用代码  
│  │  └─Throat_Detection //咽喉检测代码，分为图片检测和视频检测  
│  ├─QRcode_Serial_Test //树莓派串口接收二维码模块信息  
│  ├─Servo_Test //树莓派驱动舵机  
│  ├─STM32_Serial_Test //树莓派串口发送咽喉厚壁坐标信息至STM32   
│  └─Total //树莓派运行的全部代码  
└─STM32 //此文件夹存放了本项目中由STM32负责的代码
    ├─HAL 库激光测距通信 /STM32激光测距模块的串口通信  
    ├─HAL 激光测距 /STM32驱动激光测距模块  
    ├─SCARA机械臂 /STM32驱动SCARA机械臂  
    └─串口收发 /STM32的串口收发代码  
