#include "bsp_debug_usart.h"
#include "stdio.h"
#include "stdlib.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif 

uint16_t  t;                                 //用于计数的变量

//串口1
/*******************************************************/
uint8_t   USART1_RX_BUF[USART1_REC_LEN];     //串口1接收缓冲,最大USART_REC_LEN个字节.
uint8_t   USART1_aRxBuffer[RXBUFFERSIZE];    //串口1HAL库使用的串口接收缓冲
uint16_t  USART1_RX_STA=0;                   //串口1接收状态标记	
/*******************************************************/

//串口2
/*******************************************************/
uint8_t   USART2_RX_BUF[USART2_REC_LEN];     //串口2接收缓冲,最大USART_REC_LEN个字节.
uint8_t   USART2_aRxBuffer[RXBUFFERSIZE];    //串口2HAL库使用的串口接收缓冲
uint16_t  USART2_RX_STA=0;                   //串口2接收状态标记	
/*******************************************************/

//串口3
/*******************************************************/
uint8_t   USART3_RX_BUF[USART3_REC_LEN];     //串口3接收缓冲,最大USART_REC_LEN个字节.
uint8_t   USART3_aRxBuffer[RXBUFFERSIZE];    //串口3HAL库使用的串口接收缓冲
uint16_t  USART3_RX_STA=0;                   //串口3接收状态标记	
/*******************************************************/

//树莓派串口1通信
/*******************************************************/
uint8_t   raspberry_len;                     //树莓派传输的数据长度
uint8_t   *rapberry_BUF;                     //树莓派数据存放数组
/*******************************************************/

//激光测距串口2通信函数
/*******************************************************/
uint8_t   tof_len;                           //激光测距传输的数据长度
uint8_t   *tof_BUF;                          //激光测距数据存放数组
/*******************************************************/

//串口句柄
/*******************************************************/
UART_HandleTypeDef  UART1_Handler;           //UART1句柄
UART_HandleTypeDef  UART2_Handler;           //UART2句柄
UART_HandleTypeDef  UART3_Handler;           //UART3句柄
/*******************************************************/

  
//串口1初始化函数，参数：波特率
void uart1_init(uint32_t bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance          =  USART1;					      //USART1
	UART1_Handler.Init.BaudRate     =  bound;				          //波特率
	UART1_Handler.Init.WordLength   =  UART_WORDLENGTH_8B;    //字长为8位数据格式
	UART1_Handler.Init.StopBits     =  UART_STOPBITS_1;	      //一个停止位
	UART1_Handler.Init.Parity       =  UART_PARITY_NONE;		  //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl    =  UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode         =  UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					                  //HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (uint8_t *)USART1_aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

//串口2初始化函数，参数：波特率
void uart2_init(uint32_t bound)
{	
	//UART 初始化设置
	UART2_Handler.Instance          =  USART2;					      //USART2
	UART2_Handler.Init.BaudRate     =  bound;				          //波特率
	UART2_Handler.Init.WordLength   =  UART_WORDLENGTH_8B;    //字长为8位数据格式
	UART2_Handler.Init.StopBits     =  UART_STOPBITS_1;	      //一个停止位
	UART2_Handler.Init.Parity       =  UART_PARITY_NONE;		  //无奇偶校验位
	UART2_Handler.Init.HwFlowCtl    =  UART_HWCONTROL_NONE;   //无硬件流控
	UART2_Handler.Init.Mode         =  UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART2_Handler);					                  //HAL_UART_Init()会使能UART2
	
	HAL_UART_Receive_IT(&UART2_Handler, (uint8_t *)USART2_aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

//串口3初始化函数，参数：波特率
void uart3_init(uint32_t bound)
{	
	//UART 初始化设置
	UART3_Handler.Instance          =  USART3;					      //USART3
	UART3_Handler.Init.BaudRate     =  bound;				          //波特率
	UART3_Handler.Init.WordLength   =  UART_WORDLENGTH_8B;    //字长为8位数据格式
	UART3_Handler.Init.StopBits     =  UART_STOPBITS_1;	      //一个停止位
	UART3_Handler.Init.Parity       =  UART_PARITY_NONE;		  //无奇偶校验位
	UART3_Handler.Init.HwFlowCtl    =  UART_HWCONTROL_NONE;   //无硬件流控
	UART3_Handler.Init.Mode         =  UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART3_Handler);					                  //HAL_UART_Init()会使能UART3
	
	HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)USART3_aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

//UART底层初始化，时钟使能，引脚配置，中断配置
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)                 //如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			        //使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			      //使能USART1时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_9;			        //PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		    //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			      //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //高速
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	  //初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			        //PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	    //模式要设置为复用输入模式！	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	  //初始化PA10
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				  //使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);		//抢占优先级3，子优先级3
	}
	
	else if(huart->Instance==USART2)            //如果是串口2，进行串口2 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			        //使能GPIOA时钟
		__HAL_RCC_USART2_CLK_ENABLE();			      //使能USART1时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_2;			        //PA2
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		    //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			      //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //高速
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	  //初始化PA2

		GPIO_Initure.Pin=GPIO_PIN_3;			        //PA3
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	    //模式要设置为复用输入模式！	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	  //初始化PA10
		
		HAL_NVIC_EnableIRQ(USART2_IRQn);				  //使能USART1中断通道
		HAL_NVIC_SetPriority(USART2_IRQn,3,3);		//抢占优先级3，子优先级3
	}
	
	else if(huart->Instance==USART3)            //如果是串口3，进行串口3 MSP初始化
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			        //使能GPIOB时钟
		__HAL_RCC_USART3_CLK_ENABLE();			      //使能USART3时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_10;			        //PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		    //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			      //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //高速
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	  //初始化PB10

		GPIO_Initure.Pin=GPIO_PIN_11;			        //PB11
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	    //模式要设置为复用输入模式！	
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	  //初始化PB11
		
		HAL_NVIC_EnableIRQ(USART3_IRQn);				  //使能USART3中断通道
		HAL_NVIC_SetPriority(USART3_IRQn,3,3);		//抢占优先级3，子优先级3
	}
}


//串口中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{
		if((USART1_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART1_RX_STA&0x4000)//接收到了0x0d
			{
				if(USART1_aRxBuffer[0]!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
				else USART1_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(USART1_aRxBuffer[0]==0x0d)USART1_RX_STA|=0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=USART1_aRxBuffer[0] ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	}
	
	else if(huart->Instance==USART2)//如果是串口2
	{
		if((USART2_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
			{
				if(USART2_aRxBuffer[0]!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else USART2_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(USART2_aRxBuffer[0]==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=USART2_aRxBuffer[0] ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	}
	
	else if(huart->Instance==USART3)//如果是串口3
	{
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
			{
				if(USART3_aRxBuffer[0]!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
				else USART3_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(USART3_aRxBuffer[0]==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=USART3_aRxBuffer[0] ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	}
}
 
//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (uint8_t *)USART1_aRxBuffer, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}	

//串口2中断服务程序
void USART2_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART2_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART2_Handler, (uint8_t *)USART2_aRxBuffer, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}

//串口3中断服务程序
void USART3_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART3_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART3_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)USART3_aRxBuffer, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}

//树莓派数据提取
void raspberry_extracts(uint8_t *str) //a[0]为x坐标，a[1]为y坐标
{
	int i,j=0,flag=0,t=0,a[20];
   for(i=0;str[i]!='\0';i++){
      if(str[i]>='0'&&str[i]<='9'&&flag==0){//判断是不是第一个数字 
          flag=1;
          t=(int)str[i]-'0';//字符型强制转换为整型而且-'0'不能少 
   }
      else if(str[i]>='0'&&str[i]<='9'&&flag!=0)//判断是不是前一个数字后的连续数字 
          t=t*10+(int)str[i]-'0';
    else if(flag!=0){
     a[j]=t;
     j++;
     flag=0;
    }
   }
   if(str[i-1]>='0'&&str[i-1]<='9'){//如果最后一个字符是数字也可以将它输出 
               a[j]=t;
            j++;
          }
   printf("共有%d个数，分别是：\n",j);
   for(i=0;i<j;i++)
   printf("%d\t",a[i]);
}

//激光测距数据提取
void tof_extracts(uint8_t *str) //a[0]为 距离
{
 int i,j=0,flag=0,t=0;
 float a[20],distance;
   for(i=0;str[i]!='\0';i++){
      if(str[i]>='0'&&str[i]<='9'&&flag==0){//判断是不是第一个数字 
          flag=1;
          t=(float)str[i]-'0';//字符型强制转换为整型而且-'0'不能少 
   }
      else if(str[i]>='0'&&str[i]<='9'&&flag!=0)//判断是不是前一个数字后的连续数字 
          t=t*10+(float)str[i]-'0';
    else if(flag!=0){
     a[j]=t;
     j++;
     flag=0;
    }
   }
   if(str[i-1]>='0'&&str[i-1]<='9'){//如果最后一个字符是数字也可以将它输出 
               a[j]=t;
            j++;
          }
   printf("共有%d个数，分别是：\n",j);
   for(i=0;i<j;i++)
       printf("%f\t",a[i]);
//   HAL_UART_Transmit(&UART1_Handler,a,20,1000);
		//return(distance = a[0]);
}

//树莓派串口1通信函数
void raspberry_communication(void)
{
	if(USART1_RX_STA&0x8000)
		{					   
			raspberry_len=USART1_RX_STA&0x3fff;//得到此次接收到的数据长度
			
			rapberry_BUF=(uint8_t *) malloc ( sizeof(uint16_t) * raspberry_len);
			for(t=0;t<raspberry_len;t++)
			{
				rapberry_BUF[t] = USART1_RX_BUF[t];
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
			}
			USART1_RX_STA=0;
			//数据回显
			for(t=0;t<raspberry_len;t++)
			{
				printf("%c",rapberry_BUF[t]);
			}
			//数据提取
			raspberry_extracts(rapberry_BUF);
//			HAL_UART_Transmit(&UART1_Handler,rapberry_BUF,raspberry_len,1000);	//发送接收到的数据
		}
}

//激光测距串口2通信函数
void tof_communication(void)   
{
	//Usart_SendString("hi\r\n");
//	uint8_t TXBUFF[5] = {"h","\","r","\","n"};
//  HAL_UART_Transmit(&UART2_Handler,TXBUFF,5,1000);
	//HAL_UART_Transmit(&UART2_Handler, "hi\r\n", sizeof("hi\r\n"), 1000);
  //printf("hi\r\n");
//	if(USART2_RX_STA&0x8000)
//		{					   
			tof_len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
//			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART2_RX_BUF,tof_len,1000);	//发送接收到的数据，通过串口1发送
//			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
//			USART2_RX_STA=0;
			
			tof_BUF=(uint8_t *) malloc ( sizeof(uint16_t) * tof_len);
			for(t=0;t<tof_len;t++)
			{
				tof_BUF[t] = USART2_RX_BUF[t];
				while(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
			}
			//数据回显
			HAL_UART_Transmit(&UART1_Handler,tof_BUF,tof_len,1000);
			free(tof_BUF);
			USART2_RX_STA=0;
			//数据提取
			tof_extracts(tof_BUF);
//		}

//   printf("hi\r\n");
//	 if(USART2_RX_STA&0x8000)
//		{					   
//			tof_len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
////			printf("\r\n您发送的消息为:\r\n");
//			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART2_RX_BUF,tof_len,1000);	//发送接收到的数据，通过串口1发送
//			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
////			printf("\r\n\r\n");//插入换行
//			USART2_RX_STA=0;
//		 }


}

