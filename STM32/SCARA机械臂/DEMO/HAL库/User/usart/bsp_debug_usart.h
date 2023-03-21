#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f1xx.h"

#define RXBUFFERSIZE   1 					//���ڽ��ջ����С

//usart1
/*******************************************************/
#define USART1_REC_LEN  			200  		              //���崮��1�������ֽ��� 200
extern uint8_t    USART1_RX_BUF[USART1_REC_LEN]; 	  //����1���ջ���,���USART1_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint8_t    USART1_aRxBuffer[RXBUFFERSIZE];		//HAL��USART1����Buffer
extern uint16_t   USART1_RX_STA;         			      //����1����״̬���
extern UART_HandleTypeDef UART1_Handler; 	          //UART1���
/*******************************************************/

//usart2
/*******************************************************/
#define USART2_REC_LEN  			200  		              //���崮��2�������ֽ��� 200
extern uint8_t    USART2_RX_BUF[USART2_REC_LEN]; 	  //����2���ջ���,���USART1_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint8_t    USART2_aRxBuffer[RXBUFFERSIZE];		//HAL��USART2����Buffer
extern uint16_t   USART2_RX_STA;         			      //����2����״̬���
extern UART_HandleTypeDef UART2_Handler; 	          //UART2���
/*******************************************************/

//usart3
/*******************************************************/
#define USART3_REC_LEN  			200  		              //���崮��3�������ֽ��� 200
extern uint8_t    USART3_RX_BUF[USART3_REC_LEN]; 	  //����3���ջ���,���USART1_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint8_t    USART3_aRxBuffer[RXBUFFERSIZE];		//HAL��USART3����Buffer
extern uint16_t   USART3_RX_STA;         			      //����3����״̬���
extern UART_HandleTypeDef UART3_Handler; 	          //UART3���
/*******************************************************/


//���ڳ�ʼ������
void uart1_init(uint32_t bound);                    //����1��ʼ������
void uart2_init(uint32_t bound);                    //����2��ʼ������
void uart3_init(uint32_t bound);                    //����3��ʼ������

//����ͨ�ź���
void raspberry_extracts(uint8_t *str);              //����������ȡ
void tof_extracts(uint8_t *str);                    //����������ȡ
void raspberry_communication(void);                 //��ݮ�ɴ���1ͨ�ź���
void tof_communication(void);                       //�����മ��2ͨ�ź���
//void Usart_SendString(uint8_t *str);

#endif


