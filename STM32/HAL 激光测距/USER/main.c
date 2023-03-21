#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "stdio.h"
#include "stdlib.h"
/************************************************
 ALIENTEK ��ӢSTM32F103������ ʵ��4
 ����ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
	
	u16 t;
	u8 len;	
	u16 times=0;
//	u8 *coordinate_BUF;
	
  HAL_Init();                    	 	//��ʼ��HAL��    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	usart1_init(115200);					//��ʼ������1
	usart3_init(115200);					//��ʼ������3
	LED_Init();							//��ʼ��LED	
	KEY_Init();							//��ʼ������
    while(1)
    {
			if(USART_RX_STA&0x8000)
		{	
			
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			
			
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n");
			for(t=0;t<len;t++)
			{
//				coordinate_BUF[t] = USART_RX_BUF[t];
				printf("%c",USART_RX_BUF[t]);
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
			}
			
//			HAL_UART_Transmit(&UART3_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//���ͽ��յ�������
//			while(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}else
		{
			times++; 
			if(times%5000==0)
			{
				printf("\r\nALIENTEK ��ӢSTM32������ ����ʵ��\r\n");
				printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
			}
			if(times%200==0)printf("����������,�Իس�������\r\n");  
			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);   
		} 
    }
}
