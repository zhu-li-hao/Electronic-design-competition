# Electronic-design-competition
2022年广东省电子设计竞赛参赛作品（基于机器视觉的核酸咽拭子机器人）

__Electronic design competition__  
__├─Others__ //此文件夹存放了咽喉检测模型训练的有关文件  
__│  ├─Annotations__ //图片集标注后输出的xml文件  
__│  ├─Images__  
__│  │  ├─test__ //咽喉检测模型的测试集  
__│  │  └─train__ //咽喉检测模型的训练集     
__│  └─venv__ //训练模型的环境配置文件  
__│      └─Scripts__  
__├─Raspberry PI__ //此文件夹存放了本项目中由树莓派负责的代码  
__│  ├─Aliyun__  //二维码信息和检测结果上传至阿里云服务器   
__│  ├─EasyDL__  
__│  │  ├─Camera_Test__ //相机测试代码  
__│  │  ├─EasyDL_API__ //EasyDL的接口调用代码  
__│  │  └─Throat_Detection__ //咽喉检测代码，分为图片检测和视频检测  
__│  ├─QRcode_Serial_Test__ //树莓派串口接收二维码模块信息  
__│  ├─Servo_Test__ //树莓派驱动舵机  
__│  ├─STM32_Serial_Test__ //树莓派串口发送咽喉厚壁坐标信息至STM32   
__│  └─Total__ //树莓派运行的全部代码  
__└─STM32__ //此文件夹存放了本项目中由STM32负责的代码  
    __├─HAL 库激光测距通信__ //STM32激光测距模块的串口通信  
    __├─HAL 激光测距__ //STM32驱动激光测距模块  
    __├─SCARA机械臂__ //STM32驱动SCARA机械臂  
    __└─串口收发__ //STM32的串口收发代码  
