#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f1xx.h"

#define RXBUFFERSIZE   1 					//串口接收缓存大小

//usart1
/*******************************************************/
#define USART1_REC_LEN  			200  		              //定义串口1最大接收字节数 200
extern uint8_t    USART1_RX_BUF[USART1_REC_LEN]; 	  //串口1接收缓冲,最大USART1_REC_LEN个字节.末字节为换行符 
extern uint8_t    USART1_aRxBuffer[RXBUFFERSIZE];		//HAL库USART1接收Buffer
extern uint16_t   USART1_RX_STA;         			      //串口1接收状态标记
extern UART_HandleTypeDef UART1_Handler; 	          //UART1句柄
/*******************************************************/

//usart2
/*******************************************************/
#define USART2_REC_LEN  			200  		              //定义串口2最大接收字节数 200
extern uint8_t    USART2_RX_BUF[USART2_REC_LEN]; 	  //串口2接收缓冲,最大USART1_REC_LEN个字节.末字节为换行符 
extern uint8_t    USART2_aRxBuffer[RXBUFFERSIZE];		//HAL库USART2接收Buffer
extern uint16_t   USART2_RX_STA;         			      //串口2接收状态标记
extern UART_HandleTypeDef UART2_Handler; 	          //UART2句柄
/*******************************************************/

//usart3
/*******************************************************/
#define USART3_REC_LEN  			200  		              //定义串口3最大接收字节数 200
extern uint8_t    USART3_RX_BUF[USART3_REC_LEN]; 	  //串口3接收缓冲,最大USART1_REC_LEN个字节.末字节为换行符 
extern uint8_t    USART3_aRxBuffer[RXBUFFERSIZE];		//HAL库USART3接收Buffer
extern uint16_t   USART3_RX_STA;         			      //串口3接收状态标记
extern UART_HandleTypeDef UART3_Handler; 	          //UART3句柄
/*******************************************************/


//串口初始化函数
void uart1_init(uint32_t bound);                    //串口1初始化函数
void uart2_init(uint32_t bound);                    //串口2初始化函数
void uart3_init(uint32_t bound);                    //串口3初始化函数

//串口通信函数
void raspberry_extracts(uint8_t *str);              //用于数据提取
void tof_extracts(uint8_t *str);                    //用于数据提取
void raspberry_communication(void);                 //树莓派串口1通信函数
void tof_communication(void);                       //激光测距串口2通信函数
//void Usart_SendString(uint8_t *str);

#endif


